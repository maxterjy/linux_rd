
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un servaddr;
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, "unixconnect");

    char request[32];

    connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    while (1) {
        //o:open, d: deposit, w: withdraw, b: balance, s:shutdown
        char command = fgetc(stdin);

        sprintf(request, "%c", command);                
        write(sockfd, request, strlen(request));                
        break;
    }

    close(sockfd);

    return 0;
}
