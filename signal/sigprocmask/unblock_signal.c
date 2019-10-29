#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void handler(int sig) {
    printf("catched sig: %d, %s", sig, strsignal(sig));
}

int main( int argc, char *argv[] ) {    
    signal(SIGINT, handler);

    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);

    printf("block SIGINT\n");
    sigprocmask(SIG_BLOCK, &sigset, NULL);
    
    for(int i = 0; i < 5; i++) {
        printf("...\n");
        sleep(1);
    }

    printf("unblock SIGINT\n");
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);
    
    while (1) {
        printf("...\n");
        sleep(1);
    }
    return 0;
}
