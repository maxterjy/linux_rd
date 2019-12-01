#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

int init_listen_socket() {
    int listenfd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un servaddr;
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, "unix_connect");

    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr));

    listen(listenfd, 5);

    return listenfd;
}
