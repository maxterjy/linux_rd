#include "io.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void init_account_db(){
    int fd;

    fd = open("data", O_RDWR);

    int pos, i;
    ssize_t n;
    account_t empty = {-1, -1, -1, -1};
    account_t acc;
    int accsize = sizeof(account_t);

    if (fd < 0) { //file not exist
        fd = creat("data", S_IRUSR | S_IWUSR);

        for(i = 0; i < MAX_NUM_ACCOUNTS; i++) {

            pos = lseek(fd, 8192 * i, SEEK_SET);
            if (pos < 0) {
                break;
            }

            write(fd, &empty, accsize);
        }
    }
}
