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

void handle_client(int connectfd) {
    char buff[32];
    
    while (1) {
        int n = read(connectfd, buff, sizeof(buff));

        if (n == 0){
            printf("connection closed, connectfd = %d\n", connectfd);
            close(connectfd);            
            exit(0);
        }
        else {
            buff[n] = 0;
            printf("connectfd %d, received: %s", connectfd, buff);
        }
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

    while (1) {
        int connectfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
        
        char* ip = inet_ntoa(cliaddr.sin_addr);
        uint16_t port = ntohs(cliaddr.sin_port);
        printf("connection established, connectfd = %d, address = %s:%d\n", connectfd, ip, port);
        
        int forkid = fork();

        if (forkid == 0) { //child process
            handle_client(connectfd);
        }
        else { //parent process:
            // close(connectfd);
            //TODO: close connectfd when child process close
        }
    }

    close(listenfd);
}