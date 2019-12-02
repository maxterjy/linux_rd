
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    unlink("unixconnect");

    int listenfd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un servaddr;
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, "unixconnect");

    bind(listenfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    listen(listenfd, 5);

    char buf[32];

    while (1) {
        int connfd = accept(listenfd, NULL, 0);
        printf("connfd = %d\n", connfd);

        while (1) {
            int n = read(connfd, buf, 1);
            
            if (n <= 0) {
                close(connfd); 
                break;           
            }
            else {
                buf[n] = 0;
                printf("receive: %s\n", buf);
            }
        }
    }

    close(listenfd);

    return 0;
}
