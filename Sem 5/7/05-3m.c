#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
   int fd1[2], fd2[2], result;

   size_t size;
   char  resstring[30];

   if(pipe(fd1) < 0){
     printf("Can\'t open pipe\n");
     exit(-1);
   }

   if(pipe(fd2) < 0){
     printf("Can\'t open pipe\n");
     exit(-1);
   }

   result = fork();

   if(result < 0) {
      printf("Can\'t fork child\n");
      exit(-1);
   } else if (result > 0) {

     /* Parent process */

      close(fd1[0]);
      close(fd2[1]);

      size = write(fd1[1], "Hello, from Parent process!", 28);

      if(size != 28){
        printf("Can\'t write all string to pipe 1\n");
        exit(-1);
      }

      size = read(fd2[0], resstring, 27);

      if(size < 0){
         printf("Can\'t read string from pipe 2\n");
         exit(-1);
      }


      close(fd1[1]);
      close(fd2[0]);

      printf("Parent exit, resstring: %s\n", resstring);

   } else {

      /* Child process */

      close(fd1[1]);
      close(fd2[0]);

      size = read(fd1[0], resstring, 28);

      if(size < 0){
         printf("Can\'t read string from pipe 1\n");
         exit(-1);
      }

      size = write(fd2[1], "Hello, from Child process!", 27);

      if(size != 27){
        printf("Can\'t write all string to pipe 2\n");
        exit(-1);
      }

      printf("Child exit, resstring: %s\n", resstring);

      close(fd1[0]);
      close(fd2[1]);
   }

   return 0;
}