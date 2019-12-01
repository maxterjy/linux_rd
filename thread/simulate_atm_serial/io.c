#include "io.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void init_account_db(){
    int pos, i;
    ssize_t n;
    account_t empty = {-1, -1, -1, 1};
    account_t acc;

    int fd = open("data", O_RDWR);

    if (fd < 0) { //data not exists --> create new data
        fd = creat("data", S_IRUSR | S_IWUSR);

        for(i = 0; i < MAX_NUM_ACCOUNTS; i++) {
            pos = lseek(fd, sizeof(account_t) * i, SEEK_SET);

            if (pos < 0) {
                close(fd);
                break;
            }

            write(fd, &empty, sizeof(account_t));
        }
    }

    close(fd);
}

void log_account_db() {
    account_t *account = (account_t*)malloc(sizeof(account_t));
    int fd = open("data", O_RDWR);
    lseek(fd, 0, SEEK_SET);

    int i, pos, n;
    for(i = 0; i < MAX_NUM_ACCOUNTS; i++) {
        n = read(fd, account, sizeof(account_t));

        if (n > 0) {
            printf("acc: %d %d %d %d\n",
                account->id, account->password, account->balance, account->is_avaiable);
        }
    }

    close(fd);
}

int new_account(account_t **raccount){
    account_t *account = (account_t*)malloc(sizeof(account_t));

    int fd = open("data", O_RDWR);
    int i, pos, n;

    for(i = 0; i < MAX_NUM_ACCOUNTS; i++) {
        n = read(fd, account, sizeof(account_t));

        if (account->is_avaiable) {
            break;
        }
    }

    if (i == MAX_NUM_ACCOUNTS) {
        printf("out of slot\n");
        close(fd);
        return -1;
    }

    lseek(fd, - sizeof(account_t), SEEK_CUR);
    account->id = i;
    account->is_avaiable = 0;
    write(fd, account, sizeof(account_t));

    fsync(fd);

    close(fd);

    *raccount = account;

    return 0;
}

int delete_account(int id) {
    int fd = open("data", O_RDWR);
    lseek(fd, id * sizeof(account_t), SEEK_SET);

    account_t empty = {-1, -1, -1, 1};
    write(fd, &empty, sizeof(account_t));
    fsync(fd);

    close(fd);
}

int get_account(int id, account_t **racc){
    int fd = open("data", O_RDWR);

    account_t *a = (account_t*)malloc(sizeof(account_t));

    lseek(fd, id * sizeof(account_t), SEEK_CUR);
    read(fd, a, sizeof(account_t));

    *racc = a;

    close(fd);

    return 0;
}

int save_account(account_t *acc){
    int fd = open("data", O_RDWR);

    lseek(fd, acc->id * sizeof(account_t), SEEK_CUR);
    write(fd, acc, sizeof(account_t));

    close(fd);

    return 0;
}

int deposit(int id, int money){
    account_t *a;

    get_account(id, &a);
    a->balance += money;

    save_account(a);
}

int withdraw(int id, int money){
    account_t *a;

    get_account(id, &a);
    a->balance -= money;

    save_account(a);
}
