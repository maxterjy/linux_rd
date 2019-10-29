#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main( int argc, char *argv[] ) {    
    sigset_t newset;
    sigemptyset(&newset);
    sigaddset(&newset, SIGINT);
    sigprocmask(SIG_BLOCK, &newset, NULL);

	//SIGINT was blocked, so Ctrl+C can not interupt the process
    printf("SIGINT  (Ctrl+C) was blocked\n");
    printf("SIGSTOP (Ctrl+Z) to exit\n");

    while (1) {
        printf("looping...\n");
        sleep(1);
    }

    return 0;
}
