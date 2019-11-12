#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    printf("pid = %d\n", getpid());

    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(40000);

    bind(socketfd, (const struct sockaddr*)&servaddr, sizeof(servaddr));

    char buff[32];
    struct sockaddr_in cliaddr;
    socklen_t len;

    while (1) {
        int len = recvfrom(socketfd, buff, sizeof(buff), 0, (struct sockaddr*)&cliaddr, &len);

        if (len == 0) {
            printf("close\n");
            break;
        }
        
        char* ip = inet_ntoa(cliaddr.sin_addr);
        int port = ntohs(cliaddr.sin_port);

        buff[len] = 0;
        printf("%s:%d, %s", ip, port, buff);
    }


    close(socketfd);
}