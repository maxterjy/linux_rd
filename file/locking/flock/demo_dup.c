#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/file.h>

/* testcase
    terminal 1: sudo ./demo_dup x
    terminal 2: sudo lsof -p <pid>
*/

/* testcase
    terminal 1: sudo ./demo_dup s
    terminal 2: sudo lsof -p <pid>
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

    int fd2 = dup(fd);

    getchar();

    close(fd);
}
