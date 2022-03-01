#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

int GetDirName(int argc, char* argv[], char** dir_path);

int SearchDir(char* dir_path);

int PrintFileInfo(struct dirent* dir_entry, struct stat stat_buf);

int PrintFileType(mode_t st_mode);

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
	struct stat stat_buf;

	struct dirent* dir_entry;

	// struct 
	// {
	// 	char name[256];
	// 	char mode[10];
	// 	char* user;
	// 	char* group;
	// 	int time;
	// } file_info;

	DIR* dir = opendir(dir_path);

	if(dir == NULL)
		printf("In function SearchDir: can't open directory");

	while((dir_entry = readdir(dir)) != NULL)
	{
		// strcpy(file_info.name, dir_entry->d_name);

		if(!strcmp(dir_entry->d_name, ".") || !strcmp(dir_entry->d_name, ".."))
			lstat(dir_entry->d_name, &stat_buf);
		else
			stat(dir_entry->d_name, &stat_buf);

		PrintFileInfo(dir_entry, stat_buf);
	}

	closedir(dir);

	return 0;
}

int PrintFileInfo(struct dirent* dir_entry, struct stat stat_buf)
{
	PrintFileType(stat_buf.st_mode);

	printf("%o  ", stat_buf.st_mode & 0777);

	printf("%ld  ", stat_buf.st_nlink);

	printf("%d  ", stat_buf.st_uid);

	printf("%d  ", stat_buf.st_gid);

	printf("%6ld  ", stat_buf.st_size);

	char time[25];
	strcpy(time, ctime(&stat_buf.st_mtime));
	time[19] = '\0';
	printf("%s  ", time);

	printf("%s\n", dir_entry->d_name);

	return 0;
}

int PrintFileType(mode_t st_mode)
{
	if(S_ISLNK(st_mode))
		printf("l ");
	if(S_ISDIR(st_mode))
		printf("d ");
	if(S_ISREG(st_mode))
		printf("- ");

	return 0;
}