#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void* thread_routine (void* arg)
{
    char * arg_char = malloc(sizeof(char)*1024);
    strcpy(arg_char,(char*)arg);
    char * arg_ret = malloc(sizeof(char)*1024);
    int i=0;
    for (int j=strlen(arg_char)-1;j>=0;--j)
    {
        arg_ret[i]=arg_char[j];
        ++i;
    }
    return arg_ret;
}

int main(int arg, char* argv[])
{
    pthread_t thread;
    char thread_arg[1024];
    strcpy(thread_arg, argv[1]); 
    int x = pthread_create(&thread, NULL, thread_routine, &thread_arg);
    if (x != 0)
    {
        perror("Error at pthread_create");
        return errno;
    }

    void *retval;

    x = pthread_join(thread, &retval);
    if (x != 0)
    {
        perror("Error at pthread_join");
        return errno;
    }
    printf("%s\n", (char*)retval);

    free(retval);

}

