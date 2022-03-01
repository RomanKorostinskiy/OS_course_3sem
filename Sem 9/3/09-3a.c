#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const int LAST_MESSAGE = -1;
const int STOP = -2;
const int A_MSG = 1;
const int B_MSG = 2;

struct mymsgbuf
{
   long mtype;
   
   struct msg
   {
      int mnum;
      char mtext[80];
   } msg;

} mybuf;

int SendInfo_a(int n_of_msg, struct mymsgbuf* mybuf, int msqid);

int SendLastMsg_a(struct mymsgbuf* mybuf, int msqid);

int RecieveMsg_a(struct mymsgbuf* mybuf, int msqid);

int SendStopMsg_a(struct mymsgbuf* mybuf, int msqid);

int main(void)
{
   int msqid;
   char pathname[] = "09-1am.c";
   key_t  key;

   /* Create or attach message queue  */

   key = ftok(pathname, 0);

   if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
      printf("Can\'t get msqid\n");
      exit(-1);
   }

   /* Send information */

   SendInfo_a(5, &mybuf, msqid);

   /* Send the last message */

   SendLastMsg_a(&mybuf, msqid);

   /* Recieve messages */

   RecieveMsg_a(&mybuf, msqid);

   return 0;
}

int SendInfo_a(int n_of_msg, struct mymsgbuf* mybuf, int msqid)
{
   int len = 0;

   printf("\nA send:\n");

   for (int i = 1; i <= n_of_msg; i++)
   {

      sprintf(mybuf->msg.mtext, "This is text message from a: 1000 - 7*%d = %d", i, 1000 - 7 * i);

      mybuf->mtype = A_MSG;
      mybuf->msg.mnum = i;
      len = sizeof(struct msg);

      printf("msg: %s, text len: %ld, msg size: %ld\n", mybuf->msg.mtext, strlen(mybuf->msg.mtext), sizeof(struct msg));

      if (msgsnd(msqid, (struct msgbuf *) mybuf, len, 0) < 0){
         printf("SendInfo_a: Can\'t send message to queue\n");
         msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
         exit(-1);
      }
   }

   return 0;
}

int SendLastMsg_a(struct mymsgbuf* mybuf, int msqid)
{
   int len = sizeof(struct msg); 
   mybuf->mtype = A_MSG;
   mybuf->msg.mnum = LAST_MESSAGE;

   if (msgsnd(msqid, (struct msgbuf *) mybuf, len, 0) < 0){
      printf("SendLastMsg_a: Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
   }

   return 0;
}

int RecieveMsg_a(struct mymsgbuf* mybuf, int msqid)
{
   printf("\nA receive:\n");

   while(1)
   {
      int maxlen = 100;

      if (msgrcv(msqid, (struct msgbuf *) mybuf, maxlen, B_MSG, 0) < 0){
         printf("RecieveMsg_a: Can't receive message from queue\n");
         exit(-1);
      }

      printf("message type = %ld, info = %s, number: %d\n", mybuf->mtype, mybuf->msg.mtext, mybuf->msg.mnum);

      if (mybuf->msg.mnum == LAST_MESSAGE && mybuf->mtype == B_MSG){ //а здесь второе условие не нужно (почему?)
         printf("RecieveMsg_a: StopMessage has been sent\n");
         SendStopMsg_a(mybuf, msqid);
      }

      if (mybuf->msg.mnum == STOP && mybuf->mtype == B_MSG){ //если убрать второе условие, то не работает (почему?)
         // printf("message type = %ld, info = %s, number: %d\n", mybuf->mtype, mybuf->msg.mtext, mybuf->msg.mnum);
         printf("RecieveMsg_a: messages are stoped\n");
         msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
         exit(0);
      }
   }

   return 0;
}

int SendStopMsg_a(struct mymsgbuf* mybuf, int msqid)
{
   int len = sizeof(struct msg);
   mybuf->mtype = A_MSG;
   mybuf->msg.mnum = STOP;

   if (msgsnd(msqid, (struct msgbuf *) mybuf, len, 0) < 0){
      printf("SendStopMsg_a: Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
   }

   return 0;
}