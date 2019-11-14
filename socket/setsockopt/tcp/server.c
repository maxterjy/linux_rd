#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    printf("pid = %d\n", getpid());

    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(40000);

    if (bind(socketfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        printf("bind failed\n");
        return -1;
    }
    
    if (listen(socketfd, 32) < 0){
        printf("listen fail\n");
        return -1;
    }

    int connectfd = accept(socketfd, (struct sockaddr*)NULL, NULL);

    char *msg = "hello";
    write(connectfd, msg, strlen(msg));

    getchar();

    printf("close\n");
    close(socketfd);
}