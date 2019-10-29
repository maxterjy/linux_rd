#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void handler(int sig) {
    printf("sig = %d\n", sig);
}

int main( int argc, char *argv[] ) {
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_flags = 0;
    act.sa_handler = handler;

    sigaction(SIGINT, &act, NULL);

    sleep(3);

    raise(SIGINT);

    return 0;
}