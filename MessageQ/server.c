#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>

int msgq_id_UP,msgq_id_DOWN;
struct msgbuf
{
    long Tag;
    char message[256];
};

void handler(int signnum);
/* convert upper case to lower case or vise versa */
void conv(char *msg, int size) {
int i;
for (i=0; i<size; ++i)
    if (islower(msg[i]))
        msg[i] = toupper(msg[i]);
    else if (isupper(msg[i]))
        msg[i] = tolower(msg[i]);
}

int main (int argc, int **argv){
    signal(SIGINT,handler);

    key_t key_id_UP,key_id_DOWN;
    int rec_val,send_val;
    
    //UP Queue
    key_id_UP = ftok("UP", 37);              
    msgq_id_UP = msgget(key_id_UP, 0666 | IPC_CREAT); 

    if (msgq_id_UP == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    //DOWN queue
    key_id_DOWN = ftok("DOWN", 37);              
    msgq_id_DOWN = msgget(key_id_DOWN, 0666 | IPC_CREAT); 

    if (msgq_id_DOWN == -1)
    {
        perror("Error in create");
        exit(-1);
    }

    struct msgbuf buff;
    while(1)
    {
        /* receive all types of messages */
        rec_val = msgrcv(msgq_id_UP, &buff, sizeof(buff.message), 0, !IPC_NOWAIT);
        sleep(3);
        if (rec_val == -1)
            perror("Error in receive");
        else
            {
                conv(buff.message,strlen(buff.message));
                send_val = msgsnd(msgq_id_DOWN, &buff, sizeof(buff.message), !IPC_NOWAIT);

                if (send_val == -1)
                    perror("Error in send"); 
            }
    }

}

void handler(int signnum){

    msgctl(msgq_id_UP, IPC_RMID, (struct msqid_ds *)0);
    msgctl(msgq_id_DOWN, IPC_RMID, (struct msqid_ds *)0);
    signal(SIGINT,SIG_DFL);
    raise(SIGINT);

}