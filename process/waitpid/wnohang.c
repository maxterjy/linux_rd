#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

int main(void){
    int i, status;
    pid_t childId, endId;
    time_t when;

    childId = fork();

    if (childId == 0) {
        time(&when);
        printf("pid: %d ,child start at %s", getpid() ,ctime(&when));
        sleep(10);
        exit(0);
    }

    time(&when);
    printf("pid %d, parent start at %s", getpid(), ctime(&when));
    for(i = 0; i < 15; i++) {
        endId = waitpid(childId, &status, WNOHANG);
        time(&when);
       
        if (endId == 0) {
            printf("parent wait at %s", ctime(&when));    
            sleep(1);   
        }
        else if (endId > 0){
            printf("parent catch child TERMINATE %s", ctime(&when));

            if (WIFEXITED(status))
                printf("exit code: %d\n", WEXITSTATUS(status));
            else if (WIFSIGNALED(status))
                printf("signal code: %d\n", WTERMSIG(status));
        } else {
            printf("wait error: %s\n", strerror(errno));
        }
    }

}