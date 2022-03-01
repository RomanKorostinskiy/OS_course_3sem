#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int GetSemid (int *semid, key_t key);

char* OpenCreateFile(char* name, long int length);

int main(void)
{
	size_t length;
    char*  ptr;

    key_t key;
    int semid;
    struct sembuf sem;

    char res[128];

    length = 100000*sizeof(char);

	if((key = ftok("prog1.c",0)) < 0)
	{
	    printf("Can\'t generate key\n");
	    exit(-1);
	}

    GetSemid(&semid, key);

	ptr = OpenCreateFile("mapped.dat", length);

	if(ptr == MAP_FAILED)
	{
		printf("Mapping failed!\n");
		exit(2);
	}

	strcpy(ptr, "Hello from prog1\n");

    sem.sem_num = 0;
    sem.sem_op  = -1;
    sem.sem_flg = 0;

    if(semop(semid, &sem, 1) < 0)
    {
        printf("Can\'t wait for condition\n");
        exit(-1);
    }

	strcpy(res, ptr);

	printf("%s", res);

	munmap((void *)ptr, length);

	return 0;
}

int GetSemid (int *semid, key_t key)
{
    if((*semid = semget(key, 1, 0666|IPC_CREAT|IPC_EXCL)) < 0)
    {
        if(errno != EEXIST)
        {
            printf("Can\'t create shared memory\n");
            exit(-1);
        }
        else 
        {
            if((*semid = semget(key, 1, 0666)) < 0)
            {
                printf("Can\'t find shared memory\n");
                exit(-1);
            }
        }
    }

    return 0;
}

char* OpenCreateFile(char* name, long int length)
{
	int fd;
	char* ptr;

	fd = open(name, O_RDWR | O_CREAT, 0666);

	if(fd == -1)
	{
		printf("File open failed!\n");
		exit(1);
	}

    ftruncate(fd, length);

	ptr = (char*)mmap(NULL, length, PROT_WRITE | PROT_READ,
        MAP_SHARED, fd, 0);

    close(fd);

    return ptr;
}