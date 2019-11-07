#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdbool.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main (int argc, char* argv[]){
    printf("pid = %d\n", getpid());

    int pipeFd[2];
    printf("pipe: %d\n", pipe(pipeFd));

    struct pollfd pollfds;
    pollfds.fd = pipeFd[0];
    pollfds.events = POLLIN;  

    long nfds = 1;
    long timeout = 10000;
    
    int forkId = fork();

    if (forkId == 0) { //child
        printf("child, pid = %d\n", getpid());
        sleep(5);
        // write(pipeFd[1], "hello", 5);
        exit(0);
    }
    else {  //parent        

        int ready = poll(&pollfds, nfds, timeout);
        char buf[32];
        // read(pipeFd[0], buf, 32);
        // puts(buf);
        printf("ready = %d\n", ready);
        printf("%s\n", strerror(errno));
    }
    
    return 0;
}