#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <math.h>
#include <signal.h>

int array_len;
int* array;
pid_t p1_id, p2_id; 

// signal handler for SIGUSR1
void sigusr1_handler(int sig) {
    int status;
    pid_t pid;
    pid = wait(&status); // wait for any child to terminate
    if (WIFEXITED(status)) { // if the child exited normally
        int exit_code = WEXITSTATUS(status); // get the exit code
        printf("Child %d: Value found at position %d\n", (exit_code<(array_len/2+array_len%2))?1:2, exit_code);
    }
        kill(0, SIGKILL);       // kill all processes
}



int main(int argc, char *argv[])
{

     signal(SIGUSR1, sigusr1_handler);
    // ----------Invalid Arguments---------
    if (argc <3)
    {
        perror("To Few Arguments\n");
        exit(-1);
    }

    //---------Reading Argument-------
    int search_value = atoi(argv[1]);
    array_len = argc - 2;
    array = (int*) malloc(array_len * sizeof(int));
    for (int i = 0; i < array_len; i++)
    {
        array[i]=atoi(argv[2+i]);
    }
    
    //--------Print the process information-----
    printf("I am the parent, PID = %d\n", getpid());

    //---------------------------------------------------
   

    //-------Intializing array for grades and reading it-------------
    p1_id=fork();
    if (p1_id==-1)
    {
            perror("Fork Failed!");
            exit(-1);
    }
    if(!p1_id)
    {
        printf("I am the first child, PID = %d, PPID = %d\n", getpid(), getppid());
        int start=0;
        int end= array_len/2+array_len%2;
        for (int i = start; i < end; i++) {
            if (array[i] == search_value) {
                kill(getppid(), SIGUSR1);
                exit(i);
            }
    }
            sleep(3);
            printf("Child 1 terminates\n");
            exit(0);
    }
    else{

    p2_id=fork();
    if (p2_id==-1)
    {
            perror("Fork Failed!");
            exit(-1);
    }

    if(!p2_id)
    {
        printf("I am the second child, PID = %d, PPID = %d\n", getpid(), getppid());
        int start=array_len/2;
        int end= array_len;
        for (int i = start; i < end; i++) {
            if (array[i] == search_value) {
                kill(getppid(), SIGUSR1);
                exit(i);
            }
    }
            sleep(3);
            printf("Child 2 terminates\n");
            exit(0);
    }
    else{
            sleep(5);
            wait(NULL);
            wait(NULL);
            printf("Value not found\n");
    }
    

    }
   

    return 0;
} 


// Non-code questions:
// 1. Who will be the process executing the code in the handler?
// The process executing the code in the handler will be the parent process.
// 2. Why is it not possible that the signal handler may not receive an exit code?
// It is not possible for the signal handler to not receive an exit code because the child process don't send signal without found the search_value and send his index