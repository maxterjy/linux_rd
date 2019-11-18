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
    int sockFd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr = {
        AF_INET, htons(40000), htons(INADDR_ANY)
    };

    if (connect(sockFd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        printf("connect err: %s\n", strerror(errno));
        return -1;
    }

    char buff[32];
    char *format = "hello %d";

    int i = 0;

    while (1) {
        sprintf(buff, format, i++);

        write(sockFd, buff, strlen(buff));
        
        sleep(3);
    }

    close(sockFd);
}