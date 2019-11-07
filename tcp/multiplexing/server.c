#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/socket.h>
 
int main(int argc, char** argv) {
    
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    bzero((void*)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(40000);
    addr.sin_addr.s_addr = htonl(INADDR_ANY); 

    bind(sockFd, (struct sockaddr*)&addr, sizeof(addr));

    listen(sockFd, 16);

    int connectfd = accept(sockFd, NULL, NULL);
    printf("connection established\n\n");


    char msg[32];
    while (1) {
        int n = read(connectfd, msg, sizeof(msg));

        if (n <= 0) {
            close(connectfd);
            break;            
        }

        msg[n] = 0;
        printf("receive: %s", msg);
    }

    printf("close\n");
    close(sockFd);

    return 0;
}