#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

// implementing monitor by class account
#define NO_OF_THREADS 5
#define NO_OF_PROCESSES 2
#define SHM_SIZE 2048

struct account
{
    int balance;
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutexattr;
    pthread_condattr_t condattr;
    pthread_cond_t deposit; 
    pthread_cond_t withdraw; 
    
};

void init_account (struct account *act)
    {
        act->balance = 0;
        pthread_mutexattr_init(&act->mutexattr);
        pthread_mutexattr_setpshared(&act->mutexattr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&act->mutex,&act->mutexattr);
        pthread_condattr_init(&act->condattr);
        pthread_condattr_setpshared(&act->condattr, PTHREAD_PROCESS_SHARED);
        pthread_cond_init(&act->deposit,&act->condattr);
        pthread_cond_init(&act->withdraw,&act->condattr);
    }

void terminate_account (struct account *act)
    {
        act->balance = 0;
        pthread_mutex_destroy(&act->mutex);
        pthread_cond_destroy(&act->deposit);
        pthread_cond_destroy(&act->withdraw);
    }

void deposit_money(struct account *act, int amount)
    {
        pthread_mutex_lock(&act->mutex);
        printf("Deposit: %d\n", amount);
        act->balance += amount;
        printf("Current amount in your account: %d\n\n", act->balance);
        pthread_cond_broadcast(&act->deposit);
        pthread_mutex_unlock(&act->mutex);
    }

void withdraw_money(struct account *act, int amount)
    {
        pthread_mutex_lock(&act->mutex);
        if (act->balance < amount)
        {
            printf("You tried to withdraw: %d\n", amount);
            printf("Maximum sum to withdraw is: %d\n\n", act->balance);
        }
        else
        {
            printf("Withdraw: %d\n", amount);
            act->balance -= amount;
            printf("Current amount in your account: %d\n\n", act->balance);
        }
        pthread_cond_broadcast(&act->withdraw);
        pthread_mutex_unlock(&act->mutex);
    }

struct account *cont_curent = NULL;
int amount = 0;
pthread_mutex_t* pmutex = NULL;
pthread_mutexattr_t attrmutex;

void* thread_deposit (void* arg)
{
    int *amount_d = (int*)arg;
    deposit_money(cont_curent, *amount_d);

    return NULL;
}

void* thread_withdraw (void* arg)
{
    int *amount_w = (int*)arg;
    withdraw_money(cont_curent, *amount_w);
    
    return NULL;
}


int main() 
{
    pthread_mutexattr_init(&attrmutex);
    pthread_mutexattr_setpshared(&attrmutex, PTHREAD_PROCESS_SHARED);
    pmutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(pmutex, &attrmutex);

    char buffer[SHM_SIZE];

    const char* shm_name = "Monitor";

    int shm_fd = shm_open(shm_name,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);

    if (shm_fd == -1)
    {
        perror("Error found when trying to use shm_open");
        return errno;
    }

    int var = ftruncate(shm_fd,SHM_SIZE); 

    if (var == -1)
    {
        perror("Error found at ftruncate instruction");
        return errno;
    }

    char* shm_ptr_child=mmap(0,SHM_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,shm_fd,0);

    if (shm_ptr_child == NULL) 
    {
        perror("Error found at mmap instruction");
        return errno;
    }

    cont_curent = (struct account*)malloc(sizeof(struct account)); 
    init_account(cont_curent);
    
    pid_t pid;
    for (int j=0;j<NO_OF_PROCESSES;++j)
    {
        pid=fork(); 
        if (pid<0) 
        {
            perror("Error at fork");
            return errno;
        }
        else if (pid==0)
        {
            srand(getpid());

            pthread_mutex_lock(pmutex);
            strcpy(buffer,shm_ptr_child);
            int new_amount = atoi(buffer);
            cont_curent->balance = new_amount;
            pthread_mutex_unlock(pmutex);

            int actiune[NO_OF_THREADS], suma[NO_OF_THREADS];
            pthread_t threads[NO_OF_THREADS];
            for (int k=0;k<NO_OF_THREADS;++k)
                actiune[k]=rand()%2+1;
            for (int k=0;k<NO_OF_THREADS;++k)
                suma[k]=rand()%951+50;
            for (int i=0;i<NO_OF_THREADS;++i)
            {
                pthread_mutex_lock(pmutex);
                strcpy(buffer,shm_ptr_child);
                int new_amount = atoi(buffer);
                cont_curent->balance = new_amount;
                pthread_mutex_unlock(pmutex);
              
                if (actiune[i] == 1)
                {
                    int x = pthread_create(&threads[i], NULL, thread_deposit, &suma[i]);
                    if (x != 0)
                    {
                        perror("Error at pthread_create at deposit");
                        return errno;
                    }
                }
                else if (actiune[i] == 2)
                {
                    int x = pthread_create(&threads[i], NULL, thread_withdraw, &suma[i]);
                    if (x != 0)
                    {
                        perror("Error at pthread_create at withdraw");
                        return errno;
                    }
                }
                else
                    printf("Wrong option!");
                
                pthread_mutex_lock(pmutex);
                sprintf(shm_ptr_child,"%d\n",cont_curent->balance);
                pthread_mutex_unlock(pmutex);
            }

            for (int i=0; i<NO_OF_THREADS; i++)
            {
                int x = pthread_join(threads[i], NULL);
                if (x != 0)
                {
                    perror("Error at pthread_join");
                    return errno;
                }
            }

            break;
        }
    
    }
    
    if (pid>0)
    {
        for (int t=0;t<NO_OF_PROCESSES;++t)
            wait(NULL);
    }

    terminate_account(cont_curent);
    free(cont_curent);
    
    pthread_mutex_destroy(pmutex);
    munmap(shm_ptr_child, SHM_SIZE);
    shm_unlink(shm_name);
    return 0;
}