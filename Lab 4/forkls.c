#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main()
{
    pid_t pid = fork();

    if (pid<0)
    {
        perror("Error at fork");
        return errno;
    }
    else if (pid==0)
    {
        const char * path="/usr/bin/ls";
        char* argv[]={"ls",NULL};
        int x = execve(path,argv,NULL);
        if (x == -1)
        {
            perror("Error at execve");
            return errno;
        }
    }
    else if (pid>0)
    {
        printf("My PID=%d, Child PID=%d\n", getpid(),pid);
        wait(NULL);
        printf("Child %d finished\n", pid);
    }
}

