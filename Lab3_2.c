#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/resource.h>
#include<sys/time.h>
#include<math.h>

int Calc(int start,int end,int p,int* gs){

    // -------------Determine whether a student passed or not-----------------
    int stdNum=0;
    for (int i=start; i<end; i++)
    {
        if (gs[i]>=p)
        stdNum++;
    }
    return stdNum;
}

int main(int argc, char *argv[])
{
    FILE *grades; //file pointer
    int N, // # of TAs
    P, // Minimum grade
    S, // # of students
    pid, // proccess ID
    step, // floor(S/N)
    st,   // start index for a TA
    end; // end Indes for a TA 
  
    int *TAs= malloc(N*sizeof(int)); //TAs' Results 
    int* gs; //Total Grades (mid + final) array

    // ----------Invalid Arguments---------
    if (argc <3)
    {
        perror("To Few Arguments");
        exit(1);
    }

    //---------Reading Argument-------
    char *Path=argv[1];
    N=atoi(argv[2]);
    P=atoi(argv[3]);

    //--------Opening Grades file-----
    grades=fopen(Path,"r");
    if (grades ==NULL)
    {
        //-------Error in opeing file--------
        perror("Can Not Open the file");
        exit(1);
    }

    //--------Reading the Number of Students---------
    fscanf(grades,"%d",&S);

    //-------Intializing array for grades and reading it-------------
    gs=malloc(S*sizeof(int));
    int k=0;
    while (!feof(grades))
    {
        int mid,final;
        fscanf(grades,"%d %d",&mid,&final);
        gs[k++]=mid+final;
    }

    // --------closing file--------
    fclose(grades);

    step=S/N;
    for (int i=0; i<N; i++)
    {
        //---------the start & end Indices for each TA---------------
        st=i*step;
        end= i==N-1? S-1:st+step;

        pid=fork();
        //---------Fork Error----------
        if (pid==-1)
        {
            perror("Fork Failed!");
            exit(1);
        }
        if (pid==0) 
        { // Child
           int Res=Calc(st,end,P,gs); 
           exit(Res);
        }
        else {
            //Parent
            TAs[i]=pid;
            continue;
        }
        
    }

    //-----------Waiting for each TA to finish,then store their results-------------
    for (int i=0; i<N; i++)
    {
        int ex;
        int sid=waitpid(TAs[i],&ex,0);
        if (WIFEXITED(ex))
        {
            TAs[i]=WEXITSTATUS(ex);
        } 
    }

    //-------------printing the Final output-------------------
    for(int i=0; i<N; i++)
    {
        printf("%d ",TAs[i]);
    }
    printf("\n");
    free(TAs);
    free(gs);
    return 0;
}