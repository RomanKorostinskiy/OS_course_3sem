#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

// typedef struct Flags_t
// {

// 	int turn = 0;

// 	int ready[2] = {0, 0};

// } Flags;

int main()
{
	int *array;
	int shmid;
	int new = 1; 
	char pathname[] = "06-3am.c";
	key_t key;

	int process = 1;

	// Flags* flags;

//Массив

	if((key = ftok(pathname,0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}

	if((shmid = shmget(key, 6*sizeof(int),
		0666|IPC_CREAT|IPC_EXCL)) < 0)
	{
		if(errno != EEXIST)
		{
			printf("Can\'t create shared memory\n");
			exit(-1);
		} 
		else 
		{
			if((shmid = shmget(key, 6*sizeof(int), 0)) < 0)
			{
				printf("Can\'t find shared memory\n");
				exit(-1);
			}
		new = 0;
		}
	}

	if((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1))
	{
		printf("Can't attach shared memory\n");
		exit(-1);
	}

//Структура

	// if((key = ftok(pathname,0)) < 0)
	// {
	// 	printf("Can\'t generate key\n");
	// 	exit(-1);
	// }

	// if((shmid = shmget(key, sizeof(Flags),
	// 	0666|IPC_CREAT|IPC_EXCL)) < 0)
	// {
	// 	if(errno != EEXIST)
	// 	{
	// 		printf("Can\'t create shared memory\n");
	// 		exit(-1);
	// 	} 
	// 	else 
	// 	{
	// 		if((shmid = shmget(key, sizeof(Flags), 0)) < 0)
	// 		{
	// 			printf("Can\'t find shared memory\n");
	// 			exit(-1);
	// 		}
	// 	}
	// }

	// if((flags = (flags *)shmat(shmid, NULL, 0)) == (flags *)(-1))
	// {
	// 	printf("Can't attach shared memory\n");
	// 	exit(-1);
	// }

// array[3] - turn, array[4], array[5] - ready

	for(int i = 3; i < 6; i++)
		array[i] = 0;

	array[4 + process] = 1; //ready
	array[3] = 1 - process; //turn

	if(new)
	{
		array[0] = 0;
		array[1] = 1;
		array[2] = 1;
	}
	else
	{
		while(array[5 - process] && array[3] == 1 - process);
		
		array[1] += 1;
		for(long i = 0; i < 2000000000L; i++);
		array[2] += 1;
	}


	array[4 + process] = 0;
		
	printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n", 
		array[0], array[1], array[2]);
	
	if(shmdt(array) < 0)
	{
		printf("Can't detach shared memory\n");
		exit(-1);
	}
	
	return 0;
}