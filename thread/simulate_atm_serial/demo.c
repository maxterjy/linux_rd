#include "io.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    init_account_db();

    withdraw(1, 50);

    log_account_db();

    return 0;
}
