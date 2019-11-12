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
#include <errno.h>
#include <sys/select.h>

int main(int argc, char** argv) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9877);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        printf("connect err: %s\n", strerror(errno));
        return -1;
    }
    
    char buff[32];

    int nfds = sockfd+1;
    fd_set readfds;

    while (1) {
        printf("send: "); fflush(stdout);
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(sockfd, &readfds);

        select(nfds, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &readfds)) { //socket is readable
            printf("socket readable\n");

            int n = read(sockfd, buff, sizeof(buff));
            
            if (n == 0) {
                printf("server terminated\n");
                break;
            }
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) { //stdin is readable
            printf("stdin readable\n");

            if (fgets(buff, sizeof(buff), stdin) == NULL) { //Ctrl+D
                break;
            }

            write(sockfd, buff, strlen(buff));
        }
    }

    printf("close\n");
    close(sockfd);
}