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

int *a;
int id;

void Posao_djeteta(int m){
	for ( int j = 0; j < m; j++) (*a)++;
}

int main(int argc,char *argv[]){
	int i, n, m, pid_djeteta;
	n = atoi(argv[1]);
	m = atoi(argv[2]);
	id = shmget ( IPC_PRIVATE, sizeof(int), 0600 );
	if (id == -1) exit(1);
	a = (int *) shmat(id, NULL, 0);
	*a = 0;
	for ( i = 0; i < n; i++){
		pid_djeteta = fork ();
		switch ( pid_djeteta )
		{
			case -1:
				printf("Ne mogu stvoriti novi proces!\n");
				exit(1);
			case 0:
				Posao_djeteta(m);
				exit(0);
		}
	}
	for ( i = 0; i < n; i++)
		wait(NULL);
	printf("%d\n", *a);
	shmdt ( a );
	shmctl ( id, IPC_RMID, NULL ); 
	return 0;
}
