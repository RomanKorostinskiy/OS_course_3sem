#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>



int GetDirName(int argc, char* argv[], char** dir_path);

int SearchDir(char* dir_path);

int main(int argc, char* argv[])
{
	char* dir_path = NULL;

	dir_path = (char*)calloc(256, sizeof(char));

	GetDirName(argc, argv, &dir_path);

	SearchDir(dir_path);

	free(dir_path);

	return 0;
}

int GetDirName(int argc, char* argv[], char** dir_path)
{
	if(argc == 2)
	{
		strcpy(*dir_path, argv[1]);
	}
	else if(argc == 1)
	{
		getcwd(*dir_path, 256);
	}
	else
	{
		printf("In function GetDirName: wrong arguments\n");
		exit(-1);
	}
	
	return 0;
}

int SearchDir(char* dir_path)
{
	DIR* dir = opendir(dir_path);

	if(dir == NULL)
		printf("In function SearchDir: can't open directory");

	struct dirent* dir_entry;

	while((dir_entry = readdir(dir)) != NULL)
	{
		if(!strcmp(dir_entry->d_name, ".") || !strcmp(dir_entry->d_name, ".."))
			continue;

		printf("%s  ", dir_entry->d_name);
	}

	printf("\n");

	closedir(dir);

	return 0;
}