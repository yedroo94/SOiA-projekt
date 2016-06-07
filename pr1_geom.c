#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>

#include "common.h"

int shmid;
unsigned char *shm;

void sig(int signum)
{
	printf("\n[SIGNAL %d] Cleaning...\n", signum);
	shmdt(shm);
	shmctl(shmid, IPC_RMID, 0);
	exit(0);
}

int main(void)
{
	signal(SIGINT, sig);
	signal(SIGSEGV, sig);
	signal(SIGKILL, sig);
	setbuf(stdout, NULL);

	shmid = shmget(SHMKEY, SHMSIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	shm = (unsigned char*)shmat(shmid, 0, 0);
	printf("shm attached @%p\n", shm);

	int a0 = 1;
	int a = a0;
	for(int i = 0; i < SHMSIZE; i++)
	{
		int an = 2*a;
		a = an;
		sprintf(shm+i, "%c", an);
		printf(".");
		sleep(1);
	}

	sig(-1);
}