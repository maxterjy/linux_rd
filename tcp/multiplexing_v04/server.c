#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>
#include <sys/select.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

void handle_client(int connectfd, fd_set *monitorfds) {
    char buff[32];
    
    int n = read(connectfd, buff, sizeof(buff));

    if (n == 0){
        printf("connection closed, connectfd = %d\n", connectfd);
        FD_CLR(connectfd, monitorfds);
        close(connectfd);
        return;
    }
    else {
        buff[n] = 0;
        printf("connectfd %d, received: %s", connectfd, buff);
    }
}

int main(int argc, char** argv) {
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9877);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listenfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    listen(listenfd, 32);

    struct sockaddr_in cliaddr;
    socklen_t len;

    int nfds = listenfd + 1;
    
    fd_set readfds, monitorfds;
    FD_ZERO(&monitorfds);
    FD_SET(listenfd, &monitorfds);//add listen socket to monitor list


    while (1) {
        readfds = monitorfds;        
        select(nfds, &readfds, NULL, NULL, NULL); //monitor point

        if (FD_ISSET(listenfd, &readfds)) { //new client connection
            int connectfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
            
            char* ip = inet_ntoa(cliaddr.sin_addr);
            uint16_t port = ntohs(cliaddr.sin_port);
            printf("connection established, connectfd = %d, address = %s:%d\n", connectfd, ip, port);

            FD_SET(connectfd, &monitorfds); //add client connection socket to monitor list
            nfds = max(nfds, connectfd) + 1;
        }

        //check existing client connection data
        //0,1,2: stdin, stdout, stderr
        //3: listenfd
        //so, connectionfd start from 4
        for(int fd = 4; fd < nfds; fd++) {
            if (FD_ISSET(fd, &readfds)) {
                printf("catch %d\n", fd);
                handle_client(fd, &monitorfds);
            }
        }
    }

    close(listenfd);
}