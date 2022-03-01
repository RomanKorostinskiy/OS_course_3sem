#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct mymsgbuf
{
	long mtype;

	struct msg
	{
		int pid;
		char text[50];
	} msg;
};

int const MAX_MSG_LEN = sizeof(struct msg);

int ServerWork(int msqid);

int SendResponce(int msqid, struct mymsgbuf* msgbuf, int pid);

int main()
{
	int msqid;
   	char pathname[] = "server.c";
	key_t key;

	key = ftok(pathname, 0);

	if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
		printf("Can\'t get msqid\n");
		exit(-1);
	}

	ServerWork(msqid);

	return 0;
}

int ServerWork(int msqid)
{
	struct mymsgbuf msgbuf;

	for(int i = 1; i <= 10; i++)
	{
		int result = 0;

		result = fork();

		if (result < 0)
		{
			printf("ServerWork: Can't fork child\n");
			exit(-1);
		}

		if (result == 0)
		{
			if (msgrcv(msqid, (struct msbuf*) &msgbuf, MAX_MSG_LEN, 1, 0) < 0){
				printf("ServerWork: Can't receive message from queue\n");
				exit(-1);
			}

			printf("Server receive message from pid %d: %s\n", msgbuf.msg.pid, msgbuf.msg.text);

			// if (msgbuf.msg.pid == 1 && msgbuf.mtype == 1)
			// {
			// 	printf("ServerWork: Server rexieve stop message\n");
			// 	msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);

			// 	if (kill(getppid(), SIGKILL))
			// 	{
			// 		printf("ServerWork: Can't kill parent process\n");
			// 	}

			// 	exit(0);
			// }

			if (msgbuf.mtype == 1)
			{
				SendResponce(msqid, &msgbuf, msgbuf.msg.pid);
			}

			exit(0);
		}
	}

	printf("ServerWork: Server is started\n");

	return 0;
}

int SendResponce(int msqid, struct mymsgbuf* msgbuf, int pid)
{
	msgbuf->mtype = pid;

    sprintf(msgbuf->msg.text, "This is responce from server to pid %d", pid);

	if (msgsnd(msqid, (struct msgbuf *) msgbuf, MAX_MSG_LEN, 0) < 0){
		printf("SendResponce: Can\'t send message to queue\n");
		msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
		exit(-1);
	}

	return 0;
}