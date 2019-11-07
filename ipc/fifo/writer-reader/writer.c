#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {
    mkfifo("myfifo", 0666);

    int fd = open("myfifo", O_WRONLY);
    
    char buf[32] = "hello";
    
    while (1) {
        fgets(buf, 32, stdin);
        write(fd, buf, strlen(buf));
    }

    close(fd);
}