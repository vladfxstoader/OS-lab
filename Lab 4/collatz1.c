#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

int main(int arg, char* argv[])
{
    pid_t pid;
    for (int i=1;i<arg;++i)
    {
        pid=fork();
        if (pid<0)
        {
            perror("Error at fork");
            return errno;
        }
        else if (pid==0)
        {
            char *x = argv[i];
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
            printf("\nDone Parent %d Me %d\n", getppid(),getpid());
            break;
        }
    }
    
    if (pid>0)
    {
        printf("Starting parent %d\n",getpid());
        for (int i=1;i<arg;++i)
            wait(NULL);
    }
}

