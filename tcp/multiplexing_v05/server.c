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
#include <poll.h>

#define INFTIM -1

int max(int a, int b) {
    return (a > b) ? a : b;
}

void handle_client(struct pollfd *pollfds, int i) {
    int connectfd = pollfds[i].fd;
    char buff[32];
    
    int n = read(connectfd, buff, sizeof(buff));

    if (n == 0){
        printf("connection closed, connectfd = %d\n", connectfd);
        pollfds[i].fd = -1;
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

    struct pollfd pollfds[FD_SETSIZE];
    
    pollfds[0].fd = listenfd;//add listen socket to monitor list
    pollfds[0].events = POLLIN;

    nfds_t nfds = 1;

    while (1) {
        poll((struct pollfd*)&pollfds, nfds, INFTIM);//monitor point

        if (pollfds[0].revents & POLLIN) { //new client connection
            int connectfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);

            char* ip = inet_ntoa(cliaddr.sin_addr);
            uint16_t port = ntohs(cliaddr.sin_port);
            printf("connection established, connectfd = %d, address = %s:%d\n", connectfd, ip, port);            
            
            pollfds[nfds].fd = connectfd;
            pollfds[nfds].events = POLLIN;

            nfds++;
        }


        //read data in existing client connection 
        for(int i = 0; i < nfds; i++) {
            if (pollfds[i].fd == -1) continue;

            if (pollfds[i].revents & POLLIN) {
                printf("catch i=%d, fd=%d\n", i, pollfds[i].fd);
                handle_client((struct pollfd*)&pollfds, i);
            }
        }
    }

    close(listenfd);
}