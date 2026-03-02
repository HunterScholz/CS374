#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

int count = 0;

void lock(void){
    pthread_mutex_lock(&mutex);
}

void unlock(void){
    pthread_mutex_unlock(&mutex);
}

void *run(void *arg)
{
    (void)arg;

    while (count < 5){
        lock();
        count++;
        printf("Thread 1: count is %d\n", count);
        pthread_cond_signal(&condition);
        unlock();
        sleep(1);
    }

    return NULL;
}

int main(void)
{
    pthread_t t1;

    pthread_create(&t1, NULL, run, NULL);

    // sleep(3);

    while (count < 5){
        lock();
        pthread_cond_wait(&condition, &mutex);
        printf("Thread 2: count is %d\n", count);
        unlock();
    }

    pthread_join(t1, NULL);
}

// Calling sleep(3) in Thread 2 before anything causes Thread 1 to increment without any outputs from Thread 2.
// Thread 2 isn't executed until the count is almost finished, so it only outputs around twice. The singal call
// from Thread 1 does go through, but the code itself is stalled.