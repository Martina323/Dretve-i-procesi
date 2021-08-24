#define _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE 500
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
#include <stdbool.h>


int *zastavica;
int *pravo;
atomic_int *a;

void udi_u_KO(int i, int j){
	zastavica[i] = 1;
	while(zastavica[j] == 1)
	{
		if(*pravo == j)
		{
			zastavica[i] = 0;
			while(*pravo == j);
			zastavica[i] = 1;
		}
	}
}

void izadi_iz_KO(int i, int j){
	*pravo = j;
	zastavica[i] = 0;
}

void operacija(int i, int m){
	for ( int j = 0; j < m; j++) {
		udi_u_KO(i, 1-i);
		(*a)++;
		//sleep(1);
		izadi_iz_KO(i, 1-i);
	}
}


int main(int argc,char *argv[]){	
	int m;
	m = atoi(argv[1]);
	int id = shmget ( IPC_PRIVATE, sizeof(int), 0600 );
	if (id == -1) exit(1);
	a = (atomic_int *) shmat(id, NULL, 0);
	*a = 0;
	
	int id2 = shmget ( IPC_PRIVATE, (2+1+1)*sizeof(int), 0600 );
	if (id2 == -1) exit(1);
	zastavica = shmat(id2, NULL, 0);
	pravo = zastavica + 2;
	
	int i;
	for(int i = 0; i < 2; i++){
	      switch(fork()) {
		 case -1 :
		    printf("Ne mogu stvoriti novi proces!\n");
		    exit(1);
		 case 0 :
		    operacija(i, m);
		    exit(0);
      		}
   	}
	
	for( i=0; i<2; i++){
      		wait(NULL);
   	}
	printf("%d\n", *a);
	
	shmdt(a);
	shmdt(pravo);
	shmdt(zastavica);
	shmctl(id, IPC_RMID, NULL);
	shmctl(id2, IPC_RMID, NULL);
	
	return 0;
}

