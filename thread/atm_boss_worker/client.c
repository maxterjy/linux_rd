
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

int main(int argc, char *argv[]) {
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un servaddr;
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, "unixconnect");

    char request[32], buf[32];

    connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    FD_SET(sockfd, &readfds);
    
    int nfds = sockfd+1;

    fd_set rdfds;

    while (1) {
        FD_ZERO(&rdfds);
        rdfds = readfds;

        select(nfds, &rdfds, NULL, NULL, NULL);
        
        if (FD_ISSET(STDIN_FILENO, &rdfds)) { //data from stdin
            //o:open, d: deposit, w: withdraw, b: balance, q: quit, s:shutdown server
            int n = read(STDIN_FILENO, buf, sizeof buf);

            buf[n] = 0;
            printf("buf = %s\n", buf);

            sprintf(request, "%s", buf);                
            write(sockfd, request, strlen(request));                

            if (buf[0] == 's' || buf[0] == 'q') {
                break;
            }
        }
        else if (FD_ISSET(sockfd, &rdfds)){ //server just close
            int n = read(sockfd, buf, sizeof(buf));

            if (n <= 0) break;
        }
    }

    close(sockfd);

    return 0;
}
