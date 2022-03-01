#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[], char* envp[])
{

	printf("argc = %d\n\n", argc);

	for(int i = 0; argv[i] != NULL; i++)
	{
		printf("argv [%d] = %s\n", i, argv[i]);
	}

	for(int i = 0; argv[i] != NULL; i++)
	{
		if(!strcmp(argv[i], "-envp"))
		{
			for(int j = 0; envp[j] != NULL; j++)
			{
			printf("envp[%d] = %s\n", j, envp[j]);
			}
		}
	}

	return 0;
}