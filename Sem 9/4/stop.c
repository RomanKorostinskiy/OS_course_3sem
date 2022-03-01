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

int StopServer(int msqid, struct mymsgbuf* msgbuf);

int main()
{
	struct mymsgbuf msgbuf;
	int msqid;
   	char pathname[] = "server.c";
	key_t key;

	key = ftok(pathname, 0);

	if ((msqid = msgget(key, 0666)) < 0){
		printf("Can\'t get msqid\n");
		exit(-1);
	}

	StopServer(msqid, &msgbuf);

	return 0;
}

int StopServer(int msqid, struct mymsgbuf* msgbuf)
{
	msgbuf->mtype = 1;
	msgbuf->msg.pid = 1;

    sprintf(msgbuf->msg.text, "This message should stop server");

	if (msgsnd(msqid, (struct msgbuf *) msgbuf, MAX_MSG_LEN, 0) < 0){
		printf("SendMessage: Can\'t send message to queue\n");
		msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
		exit(-1);
	}

	return 0;
}