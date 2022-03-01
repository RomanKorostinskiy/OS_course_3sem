#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
  int   semid;
  int   shmid;
  int*  prog_cntr;
  int   new = 1;
  char pathname[]="08-1am.c";
  key_t key;
  struct sembuf mybuf;

  key = ftok(pathname, 0);

  if((shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
  {
    if(errno != EEXIST)
    {
      printf("Can\'t create shared memory\n");
      exit(-1);
    }
    else
    {
      if((shmid = shmget(key, sizeof(int), 0)) < 0)
      {
        printf("Can\'t find shared memory\n");
        exit(-1);
      }
      else
        new = 0;
    }
  }

  printf("new = %d\n", new);

  if((prog_cntr = (int *)shmat(shmid, NULL, 0)) == (int *)(-1))
  {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  if (new == 1)
    *prog_cntr = 1;
  else
    (*prog_cntr)++;

  if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0)
  {
    printf("Can\'t create semaphore set\n");
    exit(-1);
  }
    
  mybuf.sem_num = 0;
  mybuf.sem_op  = 1;
  mybuf.sem_flg = 0;
    
  if(semop(semid, &mybuf, 1) < 0)
  {
    printf("Can\'t add 1 to semaphore\n");
    exit(-1);
  }
      
  printf("In program 08-1b: The condition is set %d\n", *prog_cntr);

  if(shmdt(prog_cntr) < 0)
  {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  if(semctl(semid, 0, IPC_RMID, 0) < 0)
  {
    printf("Can't remove semaphore");
    exit(-1);
  }

    return 0;
}
