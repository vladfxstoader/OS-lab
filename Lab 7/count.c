#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#define MAX_RESOURCES 10
#define NUM_THREADS 5
int available_resources = MAX_RESOURCES;
pthread_mutex_t mutex;

int decrease_count (int count)
{
    int x = pthread_mutex_lock(&mutex);
    if (x != 0)
    {
        perror("Error at mutex_lock");
        return errno;
    }

    if (available_resources < count)
    {
        x = pthread_mutex_unlock(&mutex);
        if (x != 0)
        {
            perror("Error at mutex_unlock");
            return errno;
        }
        return -1;
    }    
    else
    {
        available_resources -= count;
        printf("Got %d resources, remaining %d\n", count, available_resources);
        x = pthread_mutex_unlock(&mutex);
        if (x != 0)
        {
            perror("Error at mutex_unlock");
            return errno;
        }
    }
    return 0;
}

int increase_count (int count)
{
    int x = pthread_mutex_lock(&mutex);
    if (x != 0)
    {
        perror("Error at mutex_lock");
        return errno;
    }
    available_resources += count;
    printf("Released %d resources, remaining %d\n", count, available_resources);
    x = pthread_mutex_unlock(&mutex);
    if (x != 0)
     {
        perror("Error at mutex_unlock");
        return errno;
    }
    return 0;
}

void* thread_routine (void* arg)
{
    int cnt = *(int*) arg;

    if (decrease_count(cnt) == 0)
    {
        increase_count(cnt);
    }

    return NULL;
}

int main ()
{
    pthread_t threads[NUM_THREADS];
    int arg[NUM_THREADS];

    int x = pthread_mutex_init(&mutex, NULL);
    if (x != 0)
    {
        perror("Error at mutex_init");
        return errno;
    }

    printf("MAX_RESOURCES=%d\n",available_resources);

    for (int i=0; i<NUM_THREADS; ++i)
    {
        arg[i]=i+1;
        x = pthread_create(&threads[i],NULL, thread_routine, &arg[i]);
        if (x != 0)
        {
            perror("Error at pthread_create");
            return errno;
        }
    }
    for (int i=0; i<NUM_THREADS; ++i)
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

