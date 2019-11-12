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
    while (1) {
        int len = recv(socketfd, buff, sizeof(buff), 0);
        
        if (len == 0) break;
        
        buff[len] = 0;
        printf("%s", buff);
    }


    close(socketfd);
}