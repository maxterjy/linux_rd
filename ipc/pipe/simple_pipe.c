#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main (int argc, char* argv[]){
    int pipepfd[2];
    pipe(pipepfd);

    char* wrbuff = "123456789";
    write(pipepfd[1], wrbuff, strlen(wrbuff));

    char rdbuff[32];
    int n = read(pipepfd[0], rdbuff, sizeof(rdbuff));
    printf("%s\n", rdbuff);

    close(pipepfd[0]);
    close(pipepfd[1]);

    return 0;
}