#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char** argv) {
    int fd = open("myfifo", O_RDONLY);

    char buf[32];

    while (1) {
        int len = read(fd, buf, 32);

        if (len <= 0)
            break;

        buf[len] = 0;
        printf("%s\n", buf);
    }

    close(fd);
}