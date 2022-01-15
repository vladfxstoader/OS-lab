#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(int arg, char* argv[])
{
    char *src = argv[1];
    char *dst = argv[2];

    struct stat stat_buf;
    if (stat(src, &stat_buf)==-1)
    {
        perror("stat failed at src");
        return errno;
    }

    int fd_src = open(src, O_RDONLY);
    if (fd_src==-1)
    {
        perror("failed at opening src");
        return errno;
    }

    char *buf = (char*)malloc(1+stat_buf.st_size*sizeof(char));
    if (buf==NULL)
    {
        perror("failed at allocating buf memory");
        return errno;
    }

    size_t bytesR;
    for (int i=0;i<stat_buf.st_size;i=i+bytesR)
    {
        bytesR=read(fd_src,buf+i,stat_buf.st_size-i);
        if (bytesR==-1)
        {
            perror("failed at reading");
            return errno;
        }
    }
    buf[stat_buf.st_size]='\0';

    int fd_dst = open(dst, O_WRONLY | O_CREAT, S_IRWXU);
    if (fd_dst==-1)
    {
        perror("failed at opening dst");
        return errno;
    }

    size_t bytesW;
    for (int i=0;i<strlen(buf);i=i+bytesW)
    {
        bytesW=write(fd_dst,buf+i,strlen(buf)-i);
        if (bytesW==-1)
        {
            perror("failed at writing");
            return errno;
        }
    }

    free(buf);
    close(fd_dst);
    close(fd_src);
    
    return 0;
}

