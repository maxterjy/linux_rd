#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdbool.h>

int main (int argc, char* argv[]){
    printf("pid = %d\n", getpid());

    struct timeval timeout;   

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    fd_set readfds;
    fd_set writefds;

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);

    int fd = STDOUT_FILENO; //open("hello.txt", O_RDWR|OCREAT);
    FD_SET(fd, &readfds);

    int nfds = fd + 1;

    printf("select\n");
    int ready = select(nfds, &readfds, &writefds, NULL, &timeout);

    bool is_read = FD_ISSET(fd, &readfds);
    printf("end, remain: %ld.%03ld, is_read:%d\n", (long)timeout.tv_sec, (long)timeout.tv_usec / 1000, is_read);

    return 0;
}