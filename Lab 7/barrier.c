#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>

#define NTHRS 5
sem_t semaphore;
pthread_mutex_t mutex;
int count=0;
void init(int n)
{
    int x = sem_init(&semaphore, 0, 0);
    if (x == -1)
    {
        perror("Error at sem_init");
        return errno;
    }
}

void barrier_point()
{
    int x = pthread_mutex_lock(&mutex);
    if (x != 0)
    {
        perror("Error at mutex_lock");
        return errno;
    }
    count = count+1;

    if (count == NTHRS)
    {
        x = pthread_mutex_unlock(&mutex);
        if (x != 0)
        {
            perror("Error at mutex_unlock");
            return errno;
        }
        for (int i=0; i<count-1; ++i)
        {
            x = sem_post(&semaphore);
            if (x == -1)
            {
                perror("Error at sem_post");
                return errno;
            }
        }
            
    }
    else
    {
        x = pthread_mutex_unlock(&mutex);
        if (x != 0)
        {
            perror("Error at mutex_unlock");
            return errno;
        }
        x = sem_wait(&semaphore);
        if (x == -1)
        {
            perror("Error at sem_wait");
            return errno;
        }
    }
}

void* tfun (void* arg)
{
    int tid = *(int*) arg;
    printf("%d reached the barrier\n", tid);
    barrier_point();
    printf("%d passed the barrier\n", tid);

    return NULL;
}

int main()
{
    pthread_t threads[NTHRS];
    int arg[NTHRS];
    int x = pthread_mutex_init(&mutex, NULL);
    if (x != 0)
    {
        perror("Error at mutex_init");
        return errno;
    }
    init(NTHRS);

    printf("NTHRS=%d\n",NTHRS);
    for (int i=0; i<NTHRS; ++i)
    {
        arg[i]=i;
        x = pthread_create(&threads[i],NULL, tfun, &arg[i]);
        if (x != 0)
        {
            perror("Error at pthread_create");
            return errno;
        }
    }
    
    for (int i=0; i<NTHRS; ++i)
    {

        x = pthread_join(threads[i],NULL);
        if (x != 0)
        {
            perror("Error at pthread_join");
            return errno;
        }
    }
    return 0;
}