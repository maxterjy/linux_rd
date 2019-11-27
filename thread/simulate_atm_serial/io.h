#define MAX_NUM_ACCOUNTS 240

typedef struct {
    int id;
    int password;
    int balance;
    int inuse;
} account_t;

void init_account_db();
