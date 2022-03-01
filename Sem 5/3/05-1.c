#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
   int     fd;
   size_t  size;
   char    string[100];

   (void)umask(0);

   if((fd = open("myfile", O_RDONLY, 0666)) < 0){
     printf("Can\'t open file\n");
     exit(-1);
   }

   size = read (fd, string, 100);

   if(size < 0){
     printf("Can\'t read all string\n");
     exit(-1);
   }

   printf("%s", string);

   if(close(fd) < 0){
     printf("Can\'t close file\n");
   }

   return 0;
}
