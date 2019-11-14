#include <sys/socket.h>
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

int main() {
    printf("pid = %d\n", getpid());

    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(40000);

    if (bind(socketfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        printf("bind failed\n");
        return -1;
    }
    
    struct timeval tv;
    tv.tv_sec = 5; //timeout is 5 seconds
    tv.tv_usec = 0;

    //set SO_RCVTIMEO
    setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    printf("wait a message for 5 seconds\n");
    char msg[32];
    int n = recv(socketfd, msg, sizeof(msg), 0);

    if (n > 0) {
        msg[n] = 0;
        printf("%s\n", msg);
    }
    else if (n < 0) {
        printf("timeout, n = %d, %s\n", n, strerror(errno));
    }
    

    printf("close\n");
    close(socketfd);
}