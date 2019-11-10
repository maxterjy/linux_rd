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
#include <poll.h>
#include <signal.h>

#define INFTIM -1

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

    struct pollfd pollfds[FD_SETSIZE];

    pollfds[0].fd = STDIN_FILENO;
    pollfds[0].events = POLLIN;

    pollfds[1].fd = sockfd;
    pollfds[1].events = POLLIN;

    nfds_t nfds = 2;

    while (1) {
        printf("send: "); fflush(stdout);

        int n = poll((struct pollfd *)&pollfds, nfds, INFTIM);

        if (pollfds[0].revents & POLLIN) {
            fgets(buff, sizeof(buff), stdin);
            write(sockfd, buff, strlen(buff));
        }

        if (pollfds[1].revents & POLLIN) {
            int len = read(sockfd, buff, sizeof(buff));
            if (len == 0) {
                break;
            }
        }
    }

    printf("close\n");
    close(sockfd);
}