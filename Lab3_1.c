#include <stdio.h>
#include <stdlib.h>

int main (int arc,char* argv[])
{
    char * str1=argv[1];
    char * str2=argv[2];

    int size1=0;
    int size2=0;

    while (str1[size1]!='\0')
    {
        size1++;
    }
    while (str2[size2]!='\0')
    {
        size2++;
    }

    if (size2!=size1)
    {
        printf("NO\n");
        return(55);
    }
    int j= size2-1;
    for (int i=0; i<size1; i++)
    {
        if (str1[i]!=str2[j--])
        {
            printf("NO\n");
        return(55);
        }
    }
    printf("Strings are reversed\n");
        return(55);
}