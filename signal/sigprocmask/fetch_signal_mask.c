#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void printSignalMask() {
    sigset_t sigmask;

    //fetch current signal mask
    sigprocmask(0, NULL, &sigmask);

    //print blocked signal mask
    printf("blocked signals:\n");
    for(int sig = 1; sig < NSIG; sig++) {
        if (sigismember(&sigmask, sig)) {
            printf("id: %d, name: %s\n", sig, strsignal(sig));
        }
    }
}

int main( int argc, char *argv[] ) {    
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGABRT);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
    
    //fetch and print current signal mask
    printSignalMask();

    return 0;
}