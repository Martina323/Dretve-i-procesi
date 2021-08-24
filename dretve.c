#include <stdio.h>
#include <signal.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/wait.h>
#include <stdatomic.h>
#include <pthread.h>

int a;
int m;

void *posao_dretve(void *status){
	for ( int j = 0; j < m; j++) a++;
	
}

int main(int argc,char *argv[]){
	int i, n, dretva;
	n = atoi(argv[1]);
	m = atoi(argv[2]);
	pthread_t thread_id[n];
	for ( i = 0; i < n; i++){
		dretva = pthread_create( &thread_id[i], NULL, posao_dretve, NULL);
		if (dretva != 0){
			printf("Ne mogu stvoriti novu dretvu!\n");
			exit(1);
		}
	}
	for(int i = 0 ; i < n ; i++) {
		pthread_join(thread_id[i], NULL);	
	}
	printf("%d\n", a);
	return 0;
}
