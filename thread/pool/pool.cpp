#include <pthread.h>
#include <queue>
#include <stdio.h>
#include <unistd.h>

using namespace std;

typedef struct {
    void* (*routine)(void *);
    void *arg;
} work_t;


int num_threads = 2;
pthread_t tid[10];

queue<work_t> works;
pthread_mutex_t mutex_works = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t workJustAdded = PTHREAD_COND_INITIALIZER;

void add_work_to_pool(work_t work) {
    pthread_mutex_lock(&mutex_works);
    works.push(work);
    pthread_cond_broadcast(&workJustAdded);
    pthread_mutex_unlock(&mutex_works);
}

void *worker_routine(void *arg) {
    while (1) {
        printf("loop\n");
        work_t w;

        pthread_mutex_lock(&mutex_works);

        while (works.size() == 0) {
            pthread_cond_wait(&workJustAdded, &mutex_works);
        }


        if (works.size() > 0) {            
            w = works.front();
            works.pop();

            pthread_mutex_unlock(&mutex_works);

            w.routine(w.arg);
        }
        else {
            pthread_mutex_unlock(&mutex_works);
        }
    }
}

void init_pool() {
    for(int i = 0; i < num_threads; i++) {
        pthread_create(&tid[i], NULL, worker_routine, NULL);
    }
}

void* foo(void *arg) {
    int n = *((int*)arg);

    printf("%ld, foo %d\n", pthread_self(), n);
    sleep(3);
    printf("%ld, foo %d, end\n", pthread_self(), n);
}

void shutdown() {
    for(int i = 0; i < num_threads; i++) {
        pthread_cancel(i);
    }
}

int main(int argc, char *argv[]) {
    init_pool();


    for(int i = 1; i <= 6; i++) {
        int *p = new int(i);
        add_work_to_pool({foo, (void*)p});
    }

    for(int i = 0; i < num_threads; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}