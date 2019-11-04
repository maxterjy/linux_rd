#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/file.h>

/* testcase
    terminal 1: sudo ./demo_fork x
    terminal 2: sudo lsof -p <parent pid>
    terminal 2: sudo lsof -p <child pid>
*/

int main( int argc, char *argv[] ) {   
    printf("pid = %d\n", getpid());

    int fd = open("hello.txt", O_RDWR | O_CREAT);
    
    int flag;
    if (argv[1][0] == 'x') {
        flag = LOCK_EX;
    }
    else {
        flag = LOCK_SH;
    }

    flock(fd, flag);
    
    int forkId = fork();
    if (forkId == 0) {//child
        printf("child, pid = %d\n", getpid());
        lseek(fd, 5, SEEK_SET);
        getchar();
        exit(0);
    } 
    
    getchar();

    close(fd);
}