#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

int main(int argc, char** argv) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;
    bzero((void*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(40000);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int connectfd = connect(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr));
    printf("input a message to send or Ctrl+C to exit\n\n");

    char msg[32];

    int nfds;
    fd_set rdset;

    int n;

    while (1) {
        printf("send: "); fflush(stdout);

        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(15, &rdset);
        FD_SET(sockfd, &rdset);
        nfds = max(sockfd, STDIN_FILENO) + 1;

        n = select(nfds, &rdset, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &rdset)){                      
            n = read(sockfd, msg, 32);

            if (n == 0) {
                printf("server terminated\n");
                break;
            }
        }

        if (FD_ISSET(STDIN_FILENO, &rdset)){            
            fgets(msg, 32, stdin);            
            write(sockfd, msg, strlen(msg));
        }
    }

    printf("close\n");
    close(sockfd);
}