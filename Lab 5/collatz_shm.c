#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/wait.h>

#define SHM_SIZE 1024

int main(int arg, char* argv[])
{
    const char* shm_name = "sharedfile";

    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (shm_fd == -1)
    {
        perror("Error at shm_open");
        return errno;
    }

    int x = ftruncate(shm_fd, SHM_SIZE);
    if (x == -1)
    {
        perror("Error at ftruncate");
        return errno;
    }

    char* shm_ptr_rdonly = mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shm_ptr_rdonly == NULL)
    {
        perror("Error at mmap");
        return errno;
    }

    int offset = 100;
    int children = arg-1;


    pid_t pid;
    for (int i = 0; i < children; i++) {
        pid = fork();
        
        if (pid == -1)
        {
            perror("Error at fork");
            return errno;
        }
        else if (pid == 0) {
            char* shm_ptr_child = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
            if (shm_ptr_child == NULL)
            {
                perror("Error at mmap");
                return errno;
            }
            char *x = argv[i+1];
            int nr=0;
            char s[100];
            for (int i=0;i<strlen(x);++i)
                nr=nr*10+(x[i]-48);
            strcat(s,x);
            strcat(s,": ");
            char aux[100];
            while (nr!=1)
            {
                sprintf(aux, "%d", nr);
                strcat(s, aux);
                strcat(s, " ");
                if (nr%2)
                    nr=3*nr+1;
                else
                    nr=nr/2;
            }
            sprintf(aux, "%d", 1);
            strcat(s, aux);
        
            sprintf(shm_ptr_child + offset * i, "%s", s);
            int y = munmap(shm_ptr_child, SHM_SIZE);
            if (y == -1)
            {
                perror("Error at munmap");
                return errno;
            }

            printf("Done Parent %d Me %d\n", getppid(), getpid());
            break;
        }
    }


    if (pid > 0) {
        printf("Starting parent %d\n", getpid());
        for(int i = 0; i < children; i++) {
            wait(NULL);
        }

        for(int i = 0; i < children; i++) {
            printf("%s\n", shm_ptr_rdonly + i * offset);
        }
        printf("Done Parent %d Me %d\n", getppid(), getpid());
    }
}

