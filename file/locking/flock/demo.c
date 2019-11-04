#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/file.h>

/* testcase: shared lock
    terminal 1: sudo ./demo s
    terminal 2: sudo ./demo s
    terminal 3: sudo lsof -p <pid>
    terminal 4: sudo lsof -p <pid>
*/

/* testcase: exclusive lock, blocking
    terminal 1: sudo ./demo x
    terminal 2: sudo ./demo x
    terminal 3: sudo lsof -p <pid>
    terminal 4: sudo lsof -p <pid>
*/

/* testcase: non-blocking lock
    terminal 1: sudo ./demo x
    terminal 2: sudo ./demo x n
    terminal 3: sudo lsof -p <pid>
    terminal 4: sudo lsof -p <pid>
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

    if (argc == 3 && argv[2][0] == 'n') {
        printf("enable non-blocking\n");
        flag |= LOCK_NB;
    }

    int status = flock(fd, flag);

    printf("status = %d\n", status);


    getchar();

    close(fd);
}