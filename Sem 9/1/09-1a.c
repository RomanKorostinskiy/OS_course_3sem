#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const int LAST_MESSAGE = 255;

int main(void)
{
   int msqid;
   char pathname[]="09-1a.c";
   key_t  key;
   int i,len;

   char message[81];

   struct mymsgbuf
   {
      long mtype;
      char mtext[81];
   } mybuf;

   /* Create or attach message queue  */

   key = ftok(pathname, 0);

   if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
      printf("Can\'t get msqid\n");
      exit(-1);
   }

   /* Send information */

   for (i = 1; i <= 5; i++){

      sprintf(message, "This is text message #%d", i);

      mybuf.mtype = 1;
      strcpy(mybuf.mtext, message);
      len = strlen(mybuf.mtext) + 1;

      if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
         printf("Can\'t send message to queue\n");
         msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
         exit(-1);
      }
   }    

   /* Send the last message */   

   mybuf.mtype = LAST_MESSAGE;   
   len         = 0;   

   if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
   }

   return 0;    
}
