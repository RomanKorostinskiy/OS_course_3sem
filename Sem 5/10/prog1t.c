#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int fd, result; size_t size; char resstring[1];
	char name[]="aaa.fifo";
	umask(0);

	if(mknod(name, S_IFIFO | 0666, 0) < 0)
	{
		printf("Can\'t create FIFO\n");
		exit(-1);
	}

	if((result = fork()) < 0){

		printf("Can\'t fork child\n");
		exit(-1);
	} 

//Parents process

	else if (result > 0) 
	{
		if((fd = open(name, O_WRONLY)) < 0)
		{
			printf("Can\'t open FIFO for writing\n");
			exit(-1);
		}

		size = write(fd, "Hello, world!", 14);
		if(size != 14)
		{
		printf("Can\'t write all string to FIFO\n");
		exit(-1);
		}
		
		close(fd);
		printf("Parent exit\n");
	}

//Child Process

	else 
	{
		if((fd = open(name, O_RDONLY)) < 0)
		{
			printf("Can\'t open FIFO for reading\n");
			exit(-1);
		}

		size = read(fd, resstring, 0);

		if(size < 0)
		{
		printf("Can\'t read string\n");
		exit(-1);
		}

		printf("Child exit");

		close(fd);
	}

	return 0;
}