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

int read_timeout(int fd, void* buff, size_t count, int nsec) {
    int ndfs = fd + 1;
    
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    struct timeval timeout;
    timeout.tv_sec = nsec;
    timeout.tv_usec = 0;

    int n = select(ndfs, &readfds, NULL, NULL, &timeout);

    if (n == 0) {
        printf("timeout\n");
        return 0;
    }
    else if (n > 0) {
        int len = read(fd, buff, count);
        return len;
    }    

    return -1;
}

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

    int nsec = 5;
    printf("waiting for data from socket for %d seconds\n", nsec);
    char msg[32];
    int n = read_timeout(socketfd, msg, sizeof(msg), nsec);
    
    if (n > 0) {
        printf("msg = %s\n", msg);
    }

    printf("close\n");
    close(socketfd);
}