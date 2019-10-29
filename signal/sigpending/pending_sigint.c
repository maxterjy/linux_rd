#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void showPendingSignal(int sig) {
    printf("check pending signal\n");
    sigset_t pendingSet;
    sigpending(&pendingSet);

    if (sigismember(&pendingSet, SIGINT)) {
        printf("SIGINT is pending\n");
    }
    else {
        printf("SIGINT is NOT pending\n");
        printf("Ctrl+C to add it to pending\n");
    }
}

int main( int argc, char *argv[] ) {    
    signal(SIGQUIT, showPendingSignal);

    sigset_t blockSet;
    sigemptyset(&blockSet);
    sigaddset(&blockSet, SIGINT);
    sigprocmask(SIG_BLOCK, &blockSet, NULL);

    printf("Ctrl+\\ to check if SIGINT is pending\n");
    while (1) {
        printf(".\n");
        sleep(2);
    }

    return 0;
}