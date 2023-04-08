#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/sem.h>
#include <sys/shm.h>

/* arg for semctl system calls. */
union Semun
{
    int val;               /* value for SETVAL */
    struct semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
    int *array;            /* array for GETALL & SETALL */
    struct seminfo *__buf; /* buffer for IPC_INFO */
    void *__pad;
};

void down(int sem)
{
    struct sembuf p_op;

    p_op.sem_num = 0;
    p_op.sem_op = -1;
    p_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &p_op, 1) == -1)
    {
        perror("Error in down()");
        exit(-1);
    }
}

void up(int sem)
{
    struct sembuf v_op;

    v_op.sem_num = 0;
    v_op.sem_op = 1;
    v_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &v_op, 1) == -1)
    {
        perror("Error in up()");
        exit(-1);
    }
}

/* convert upper case to lower case or vise versa */
void conv(char *msg, int size)
{
    int i;
    for (i = 0; i < size; ++i)
        if (islower(msg[i]))
            msg[i] = toupper(msg[i]);
        else if (isupper(msg[i]))
            msg[i] = tolower(msg[i]);
}


int main(int argc, int **argv)
{

    int shmid;
    key_t shm_key,sem1_key,sem2_key;
    union Semun semun;

    sem1_key = ftok("sem1", 2);              
    sem2_key = ftok("sem2", 3);              

    int sem1 = semget(sem1_key, 1, 0666 | IPC_CREAT);
    int sem2 = semget(sem2_key, 1, 0666 | IPC_CREAT);

    if (sem1 == -1 || sem2 == -1)
    {
        perror("Error in create sem");
        exit(-1);
    }
    down (sem1);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////


    shm_key = ftok("shm", 1);              

    // Get id for shared memory segment
    shmid = shmget(shm_key, 4096, IPC_CREAT | 0666);

    if (shmid == -1)
    {
        perror("Error in create");
        exit(-1);
    }
    else
    {
        printf("\nShared memory ID = %d\n", shmid);
    }

    // Attach shared memory segment
    char *shmaddr = shmat(shmid, (void *)0, 0);
    if (shmaddr == (void*)-1)
    {
        perror("Error in attach in writer");
        exit(-1);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////


    char input[256];

    // Write input to shared memory
    strcpy(input, shmaddr);
    conv(input,256);
    strcpy(shmaddr, input);
    up(sem2);


}
