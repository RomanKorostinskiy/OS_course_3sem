#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int READ = 0;
const int WRITE = 1;

int GetSemid (int *semid, key_t key, struct sembuf* sem1);

int ParentProcess(int fd1[], int fd2[], int semid1, int semid2, struct sembuf* sem1, struct sembuf* sem2, int num_of_call);

int ChildProcess(int fd1[], int fd2[], int semid1, int semid2, struct sembuf* sem1, struct sembuf* sem2, int num_of_call);

int main()
{
    int fd1[2], fd2[2], result;

    char    pathname[] = "05-3m.c";
    key_t   key;
    int     semid1, semid2;
    struct  sembuf sem1, sem2;

    if((key = ftok("05-3m.c",0)) < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    GetSemid(&semid1, key, &sem1);

    sem1.sem_num = 0;
    sem1.sem_op  = 1;
    sem1.sem_flg = 0;

    if(semop(semid1, &sem1, 1) < 0)
    {
        printf("Can\'t wait for condition\n");
        exit(-1);
    }

    if((key = ftok("05-3m_copy.c",0)) < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    GetSemid(&semid2, key, &sem2);

    if(pipe(fd1) < 0)
    {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    if(pipe(fd2) < 0)
    {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    result = fork();

    for(int i = 0; i < 6; i++)
    {
        if(result < 0)
        {
            printf("Can\'t fork child\n");
            exit(-1);
        }
        else if (result > 0) 
        {
            ParentProcess(fd1, fd2, semid1, semid2, &sem1, &sem2, i);
        }
        else
        {
            ChildProcess(fd1, fd2, semid1, semid2, &sem1, &sem2, i);
        }
   }

    close(fd1[READ]);
    close(fd1[WRITE]);
    close(fd2[READ]);
    close(fd2[WRITE]);

    if (result > 0)
    {
        if(semctl(semid1, 0, IPC_RMID, 0) < 0)
        {
            printf("Can't remove semaphore 1\n");
            // exit(-1);
        }

        if(semctl(semid2, 0, IPC_RMID, 0) < 0)
        {
            printf("Can't remove semaphore 2\n");
            // exit(-1);
        }
    }

   return 0;
}

int GetSemid (int *semid, key_t key, struct sembuf* sem1)
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
            if((*semid = semget(key, 1, 0)) < 0)
            {
                printf("Can\'t find shared memory\n");
                exit(-1);
            }
        }
    }

    return 0;
}

int ParentProcess(int fd1[], int fd2[], int semid1, int semid2, struct sembuf* sem1, struct sembuf* sem2, int num_of_call)
{
    char* parent_msg = "Hello, from Parent process!";
    int   parent_msg_len = strlen(parent_msg) + 1; //TODO: Почему не работает без +1?

    char* child_msg = "Hello, from Child process!";
    int   child_msg_len = strlen(child_msg) + 1; //TODO: Почему не работает без +1?

    size_t size;
    char   resstring[50];

   /* Parent process */

    sem1->sem_num = 0;
    sem1->sem_op  = -1;
    sem1->sem_flg = 0;

    if(semop(semid1, sem1, 1) < 0)
    {
        printf("Can\'t wait for condition\n");
        exit(-1);
    }

    size = write(fd1[WRITE], parent_msg, parent_msg_len);

    printf("Parent write: size = %ld %d \n", size, num_of_call + 1);

    if(size != parent_msg_len) //TODO: почему не может записать?
    {
        printf("Can\'t write all string to pipe 1\n");
        exit(-1);
    }

    sem2->sem_num = 0;
    sem2->sem_op  = -1;
    sem2->sem_flg = 0;

    if(semop(semid2, sem2, 1) < 0)
    {
        printf("Can\'t wait for condition\n");
        exit(-1);
    }


    size = read(fd1[READ], resstring, child_msg_len);

    if(size < 0)
    {
       printf("Can\'t read string from pipe 2\n");
       exit(-1);
    }

    printf("Parent exit, resstring: %s %d\n", resstring, num_of_call + 1);

    sem1->sem_num = 0;
    sem1->sem_op  = 1;
    sem1->sem_flg = 0;

    if(semop(semid1, sem1, 1) < 0)
    {
        printf("Can\'t wait for condition\n");
        exit(-1);
    }

   /* Parent process */

    return 0;
}

int ChildProcess(int fd1[], int fd2[], int semid1, int semid2, struct sembuf* sem1, struct sembuf* sem2, int num_of_call)
{
    char* parent_msg = "Hello, from Parent process!";
    int   parent_msg_len = strlen(parent_msg) + 1; //TODO: Почему не работает без +1?

    char* child_msg = "Hello, from Child process!";
    int   child_msg_len = strlen(child_msg) + 1; //TODO: Почему не работает без +1?

    size_t size;
    char   resstring[50];

    /* Child process */

    // wait(1000);

    size = read(fd1[READ], resstring, parent_msg_len);

    printf("Child read: size = %ld %d \n", size, 1 + num_of_call);

    if(size < 0) //TODO: Почему сюда не заходит?
    {
        strcpy(resstring, "Error");
        printf("Can\'t read string from pipe 1\n");
        exit(-1);
    }

    sem2->sem_num = 0;
    sem2->sem_op  = 1;
    sem2->sem_flg = 0;

    if(semop(semid2, sem2, 1) < 0) //TODO: Почему тут не ломается?
    {
        printf("Can\'t wait for condition\n");
        exit(-1);
    }

    size = write(fd1[WRITE], child_msg, child_msg_len);

    if(size != child_msg_len)
    {
      printf("Can\'t write all string to pipe 2\n");
      exit(-1);
    }

    printf("Child exit, resstring: %s %d\n", resstring, 1 + num_of_call);

    sem1->sem_num = 0;
    sem1->sem_op  = -1;
    sem1->sem_flg = 0;

    if(semop(semid1, sem1, 1) < 0)
    {
        printf("Can\'t wait for condition 1\n");
        exit(-1);
    }

    sem1->sem_num = 0;
    sem1->sem_op  = 1;
    sem1->sem_flg = 0;

    if(semop(semid1, sem1, 1) < 0)
    {
        printf("Can\'t wait for condition 2\n");
        exit(-1);
    }

    /* Child process */

    return 0;
}