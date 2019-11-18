#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_key_t key;

void destructor(void* param) {
	printf("destructor\n");
}

void test(){
	int* p = pthread_getspecific(key);
	printf("get specific: %p\n", p);
}

void* routine(void* param) {
	printf("routine\n");
	
	int *p = malloc(sizeof(int));

	printf("set specific: %p\n", p);

	pthread_setspecific(key, p);

	test();
}


int main(int argc, char* argv[]) {
	
	pthread_key_create(&key, destructor);
	printf("%d\n", key);

	pthread_t tid;
	pthread_create(&tid, NULL, routine, NULL);

	pthread_t tid2;
	pthread_create(&tid2, NULL, routine, NULL);

	pthread_join(tid, NULL);
	pthread_join(tid2, NULL);
	pthread_key_delete(key);
	return 0;	
}