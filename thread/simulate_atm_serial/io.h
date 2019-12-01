#define MAX_NUM_ACCOUNTS 5

typedef struct {
    int id;
    int password;
    int balance;
    int is_avaiable;
} account_t;

void init_account_db();
void log_account_db();

int new_account(account_t **racc);
int save_account(account_t *acc);
int delete_account(int id);

int get_account(int id, account_t **racc);

int deposit(int id, int money);
int withdraw(int id, int money);
