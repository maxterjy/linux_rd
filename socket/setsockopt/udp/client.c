#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>

int main() {
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(40000);

    if (connect(socketfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        printf("connect failed\n");
        return -1;
    }

    char* msg = "hello";
    int n = sendto(socketfd, msg, sizeof(msg), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));


    printf("close\n");
    close(socketfd);
}