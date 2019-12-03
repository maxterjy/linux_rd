
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

//create a thread per request

typedef struct{
    int conn;
    char req_buf[32];
} workorder_t;

typedef struct{
    int num_active;
    pthread_cond_t thread_exit_cv;
    pthread_mutex_t mutex;
} thread_info_t;


pthread_mutex_t global_data_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t account_mutex[32];
pthread_mutex_t open_account_mutex = PTHREAD_MUTEX_INITIALIZER;

thread_info_t thread_info;
workorder_t *pworkorder;

void* process_request_routine(void* args);
void process_request(int connfd, char* request);

void open_account();
void deposit();
void withdraw();
void balance();
void close_connection();

int main(int argc, char *argv[]) {
    unlink("unixconnect");    

    int listenfd = socket(AF_UNIX, SOCK_STREAM, 0);

    //multiplexing: add listenfd to observefds
    int nfds = listenfd+1;
    
    fd_set observefds;
    FD_ZERO(&observefds);
    FD_SET(listenfd, &observefds);


    struct sockaddr_un servaddr;
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, "unixconnect");

    bind(listenfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    listen(listenfd, 5);

    char buf[32];

    fd_set readfds;
    int nr, ret;
    
    pthread_mutex_init(&thread_info.mutex, NULL);
    pthread_cond_init(&thread_info.thread_exit_cv, NULL);

    while (1) {
        //multiplexing: monitor observefds
        FD_ZERO(&readfds);
        readfds = observefds;
        nr = select(nfds, &readfds, NULL, NULL, NULL);
        
        if (FD_ISSET(listenfd, &readfds)) { //new connection
            int connfd = accept(listenfd, NULL, 0);
            printf("new connection: connfd = %d\n", connfd);
            
            FD_SET(connfd, &observefds);
            
            if (nfds < connfd + 1) {
                nfds = connfd + 1;
            }
        }
        else {
            //data from connected client
            for(int i = listenfd+1; i < nfds; i++) {
                if (FD_ISSET(i, &readfds)) {
                    ret = read(i, buf, sizeof(buf));

                    if (ret <= 0) {
                        printf("close %d\n", i);

                        close(i);
                        FD_CLR(i, &observefds);
                        break;
                    }                 
                    
                    buf[ret] = 0;

                    //handle shutdown request
                    if (buf[0] == 's') {
                        printf("shutdown\n");
                        printf("waiting for in-progress threads\n");
                        pthread_mutex_lock(&thread_info.mutex);
                        
                        while (thread_info.num_active > 0) {
                            pthread_cond_wait(&thread_info.thread_exit_cv, &thread_info.mutex);
                        }

                        pthread_mutex_unlock(&thread_info.mutex);

                        close(listenfd);
                        printf("shutdown, completely\n");

                        exit(0);
                    }


                    pthread_t tid;
                    workorder_t data;
                    data.conn = i;
                    strcpy(data.req_buf, buf);

                    //spawn thread to handle other request
                    pthread_mutex_lock(&thread_info.mutex);
                    thread_info.num_active++;
                    pthread_mutex_unlock(&thread_info.mutex);

                    pthread_create(&tid, NULL, process_request_routine, &data);
                    pthread_detach(tid);
                }
            }
        }
    }

    close(listenfd);

    return 0;
}


void* process_request_routine(void* args) {
    workorder_t data = *(workorder_t*)args;
    int connfd = data.conn;
    char* request = data.req_buf;
    process_request(connfd, request);

    pthread_mutex_lock(&thread_info.mutex);
    thread_info.num_active--;
    pthread_cond_signal(&thread_info.thread_exit_cv);
    pthread_mutex_unlock(&thread_info.mutex);
}

void process_request(int connfd, char* request) {
    printf("process request, connfd = %d, request = %s\n", connfd, request);

    switch (request[0])
    {
        case 'o':
            open_account();
        break;

        case 'd':
            deposit();
        break;

        case 'w':
            withdraw();
        break;

        case 'b':
            balance();
        break;

        case 'q':
            close_connection();
    
    default:
        break;
    }

}

void open_account() {
    printf("open account\n");
    sleep(5);
    printf("open account, completely\n");
}

void deposit() {
    printf("deposit\n");
    sleep(5);
    printf("deposit, completely\n");
}

void withdraw() {
    printf("withdraw\n");
    sleep(5);
    printf("withdraw, completely\n");
}

void balance() {
    printf("balance\n");
    sleep(5);
    printf("balance, completely\n");
}

void close_connection() {
    printf("close_connection\n");
    sleep(5);
    printf("close_connection, completely\n");
}