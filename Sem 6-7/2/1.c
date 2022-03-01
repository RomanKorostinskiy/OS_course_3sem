#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
	key_t key = 0;
	int shmid = 0;
	const char* file_name = "1.c";

	int fd = 0;
	struct stat buff;
	int file_size;

	char* array = NULL;

// Открываем файл

	if((fd = open("1.c", O_RDONLY, 0666)) < 0)
	{
    	printf("Can\'t open file\n");
    	exit(-1);
   	}

// Получаем размер файла

   	fstat(fd, &buff);
   	file_size = buff.st_size;

	if ((key = ftok(file_name, 0)) < 0)
	{
		printf("Can't get key\n");
		exit(-1);
	}

	if ((shmid = shmget(key, file_size, 0666|IPC_CREAT|IPC_EXCL)) < 0)
	{
		if(errno != EEXIST)
		{
			printf("Can\'t create shared memory\n");
			exit(-1);
		}
		else 
		{
			if((shmid = shmget(key, 3*sizeof(int), 0)) < 0)
			{
				printf("Can\'t find shared memory\n");
				exit(-1);
			}
		}
	}

	if((array = (char *)shmat(shmid, NULL, 0)) == (char *)(-1))
	{
		printf("Can't attach shared memory\n");
		exit(-1);
	}

//Записываем файл в массив

   	if (read(fd, array, file_size) != file_size)
   	{
		printf("Can't read from file\n");
		exit(-1);
   	}

	if(shmdt(array) < 0)
	{
		printf("Can't detach shared memory\n");
		exit(-1);
	}

   	return 0;
}