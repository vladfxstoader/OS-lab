#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
    size_t bytesW;
    char *str = "Hello, world!\n";

    for (int i=0; i<strlen(str); i=i+bytesW)
    {
        bytesW=write(STDOUT_FILENO, str+i, strlen(str)-i);
    
        if (bytesW==-1)
        {
            perror("failed to write");
            return errno;
        }
    }
    
    return 0;
}

