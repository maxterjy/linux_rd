#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char** argv) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;
    bzero((void*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(40000);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr));
    printf("input a message to send or Ctrl+C to exit\n\n");
    
    char msg[32];

    while (1) {
        printf("send: ");
        fgets(msg, 32, stdin);

        write(sockfd, msg, strlen(msg));
    }

    printf("close\n");
    close(sockfd);
}