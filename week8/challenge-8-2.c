#include <stdio.h>
#include <pthread.h>

#define ELEMENTS 100000
#define THREADS 4

int a[ELEMENTS] = {0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

volatile int spinlock1 = 0;

void lock(void) {
    while (__sync_lock_test_and_set(&spinlock1, 1))
        ;
}

void unlock(void) {
    __sync_synchronize(); // Memory barrier.
    spinlock1 = 0;
}

void *run(void *arg)
{
    (void)arg;

    pthread_mutex_lock(&mutex);
    // lock();

    for (int i = 0; i < ELEMENTS; i++){
        // pthread_mutex_lock(&mutex);
        a[i]++;
        // pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_unlock(&mutex);
    // unlock();

    return NULL;
}

int main(void)
{
    pthread_t thread[THREADS];

    for (int i = 0; i < THREADS; i++)
        pthread_create(thread + i, NULL, run, NULL);

    for (int i = 0; i < THREADS; i++)
        pthread_join(thread[i], NULL);

    int expected = THREADS;

    for (int i = 0; i < ELEMENTS; i++)
        if (a[i] != expected)
            printf("a[%d] = %d, expected %d\n", i, a[i], expected);
}

// The mutexes being outside the loop made the code faster than being inside the loop. This is because
// each thread did not need to lock and unlock after every single iteration, it only had to do so once.

// I think that the spin locks take longer than mutexes because they are constantly checking within the
// while loop. Doing so for each thread as they are also performing run slows down the code, while mutexes
// do not have constant checks going on.