#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <pthread.h>


void* handleClient(void *arg) {
    pthread_detach(pthread_self());

    int connectFD = (int)arg;    
    printf("tid=%ld, handleClient connectFd=%d \n", pthread_self(), connectFD);    
    
    char buff[32];

    while (1)
    {
        int n = read(connectFD, buff, sizeof(buff));

        if (n > 0) {
            buff[n] = 0;
            printf("tid=%ld, receive: %s\n", pthread_self(), buff);
        }
        else {
            printf("close\n");
            close(connectFD);
            break;
        }
    }
    
}

int main() {
    struct sockaddr_in servaddr = {
        AF_INET, htons(40000), htons(INADDR_ANY)
    };

    int listenFD = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(listenFD, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        printf("bind err: %s\n", strerror(errno));
        return -1;
    }

    if (listen(listenFD, 16) < 0) {
        printf("listen err: %s\n", strerror(errno));
        return -1;
    }

    while (1) {
        int connectFD = accept(listenFD, NULL, NULL);

        pthread_t tid;
        pthread_create(&tid, NULL, handleClient, (void*)connectFD);
    }

    sleep(7);

}