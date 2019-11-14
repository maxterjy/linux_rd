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
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(40000);

    if (connect(socketfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        printf("connect failed\n");
        return -1;
    }

    struct timeval tv;
    tv.tv_sec = 5; //timeout is 5 seconds
    tv.tv_usec = 0;

    //set SO_RCVTIMEO
    setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    int nsec = 5;
    printf("waiting for data from socket for %d seconds\n", nsec);
    char msg[32];
    int n = read(socketfd, msg, sizeof(msg));

    if (n > 0) {
        msg[n] = 0;
        printf("%s\n", msg);
    }
    else {
        printf("n = %d, %s\n", n, strerror(errno));
    }
    


    printf("close\n");
    close(socketfd);
}