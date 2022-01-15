#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

int main(int arg, char* argv[])
{
    pid_t pid = fork();

    if (pid<0)
    {
        perror("Error at fork");
        return errno;
    }
    else if (pid==0)
    {
        char *x = argv[1];
        int nr=0;
        for (int i=0;i<strlen(x);++i)
            nr=nr*10+(x[i]-48);
        printf("%d: ",nr);
        while (nr!=1)
        {
            printf("%d ",nr);
            if (nr%2)
                nr=3*nr+1;
            else
                nr=nr/2;
        }
        printf("1\n");
    }
    else if (pid>0)
    {
        wait(NULL);
        printf("Child %d finished\n", pid);
    }
}

