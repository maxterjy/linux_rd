#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void handler(int sig, siginfo_t* info, void* context) {
    printf("sig = %d, pid = %d\n", sig, info->si_pid);
}

int main( int argc, char *argv[] ) {
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = handler;

    sigaction(SIGINT, &act, NULL);

    sleep(3);

    raise(SIGINT);

    return 0;
}