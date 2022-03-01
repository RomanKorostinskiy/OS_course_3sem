#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
  int   semid;
  char pathname[]="08-1am.c";
  key_t key;
  struct sembuf mybuf;
  
  key = ftok(pathname, 0);
  
  if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
    printf("Can\'t create semaphore set\n");
    exit(-1);
  }
  
  mybuf.sem_num = 0;
  mybuf.sem_op  = -5;
  mybuf.sem_flg = 0;
  
  if(semop(semid, &mybuf, 1) < 0){
    printf("Can\'t wait for condition\n");
    exit(-1);
  }  
    
  printf("In program 08-1a: The condition is present\n");

  if(semctl(semid, 0, IPC_RMID, 0) < 0)
  {
    printf("Can't remove semaphore");
    exit(-1);
  }

  return 0;
}
