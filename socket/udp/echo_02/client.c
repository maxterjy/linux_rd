#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(40000);

    char buff[32];

    while(1) {
        fgets(buff, sizeof(buff), stdin);    

        sendto(socketfd, buff, strlen(buff), 0, (const struct sockaddr*)&servaddr, sizeof(servaddr));
    }

    close(socketfd);
}