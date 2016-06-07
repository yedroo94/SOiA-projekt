#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"

int stime;

void suicide(char c)
{
	int t = time(NULL) - stime;
	printf("\nGot 5 occurencies of %d\n%d seconds elapsed.\n", c, t);
	exit(0);
}

int main(void)
{
	int shmid = shmget(SHMKEY, 1, S_IRUSR | S_IWUSR);

	unsigned char *shm = (char*)shmat(shmid, 0, 0);
	printf("shm attached @%p\n", shm);

	stime = time(NULL);
	for(;;)
	{
		unsigned char last = 0;
		short count = 0;
		for(int i = 0; i < SHMSIZE; i++)
		{
			unsigned char curr = *(shm+i);
			if(curr == 0)
				break;
			if(curr == last)
				count++;
			if(count == 5)
				suicide(curr);
			last = curr;
			printf("%d ", curr);
		}
		printf("\n\n");
		sleep(1);
	}

	return 0;
}