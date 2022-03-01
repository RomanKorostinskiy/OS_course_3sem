#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct msgbuf
{
	long type;
	int msg;
};

int RecieveStopMessage(int msqid);

int main()
{
	key_t key;
	int msqid;
	char pathname[] = "sem.c";

	key = ftok(pathname, 0);

	if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
	{
		printf("Can't get msqid\n");
		exit(-1);
	}

	RecieveStopMessage(msqid);

	return 0;
}

int RecieveStopMessage(int msqid)
{
	struct msgbuf msgbuf;

	int msg_len = sizeof(int);

	msgbuf.type = 1;
	msgbuf.msg = 1;

	if (msgrcv(msqid, (struct msbuf*) &msgbuf, msg_len, 255, 0) < 0)
	{
		printf("RecieveStopMessage: Can't receive message from queue\n");
		exit(-1);
	}

	if(msgbuf.type == 255)
	{
		printf("Message queue has been removed\n");
		msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
		exit(0);
	}


	return 0;
}