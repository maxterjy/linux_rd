#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

pthread_once_t once_control;

void once_routine() {
	printf("once routine\n");
} 

void* routine(void* param) {
	printf("thread %ld, routine 1\n", pthread_self());

	pthread_once(&once_control, once_routine);

	printf("thread %ld, routine 2\n", pthread_self());
}

int main(int argc, char* argv[]) {
	pthread_t tid1, tid2;

	pthread_create(&tid1, NULL, routine, NULL);
	pthread_create(&tid2, NULL, routine, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0;	
}