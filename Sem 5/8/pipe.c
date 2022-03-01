#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int fd[2], size = 0;

  if(pipe(fd) < 0)
  {
     printf("Can\'t open pipe\n");
     exit(-1);
  }

  int n = 1;

  while (n == 1)
  {
    n = write(fd[1], "0", 1);
    if (n == 1)
      size++;
    printf("size: %d\n", size);
  }

  printf("size of pipe: %d \n", size);
  
  return 0;
}