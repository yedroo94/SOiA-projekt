#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

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
	srand(time(NULL));
	signal(SIGINT, sig);
	signal(SIGSEGV, sig);
	signal(SIGKILL, sig);
	setbuf(stdout, NULL);

	shmid = shmget(SHMKEY, SHMSIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	shm = (unsigned char*)shmat(shmid, 0, 0);
	printf("shm attached @%p\n", shm);

	for(int i = 0; i < SHMSIZE; i++)
	{
		sprintf(shm+i, "%c", (rand()%255)+1); // dont save 0's
		printf(".");
		sleep(1);
	}

	sig(-1);
}