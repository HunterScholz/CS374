#include <stdio.h>
#include <pthread.h>

#define ELEMENTS 50
#define THREADS 8

pthread_mutex_t lock_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_b = PTHREAD_MUTEX_INITIALIZER;

int a[ELEMENTS] = {0};
int b[ELEMENTS] = {0};

void *run1(void *arg)
{
    (void)arg;

    for (int i = 0; i < ELEMENTS; i++) {
        pthread_mutex_lock(&lock_b);
        pthread_mutex_lock(&lock_a);
        a[i]++;
        pthread_mutex_unlock(&lock_a);
        pthread_mutex_unlock(&lock_b);
    }

    return NULL;
}

void *run2(void *arg)
{
    (void)arg;

    for (int i = 0; i < ELEMENTS; i++) {
        pthread_mutex_lock(&lock_b);
        pthread_mutex_lock(&lock_a);
        a[i]++;
        pthread_mutex_unlock(&lock_a);
        pthread_mutex_unlock(&lock_b);
    }

    return NULL;
}

int main(void)
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, run1, NULL);
    pthread_create(&t2, NULL, run2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}


// In both for loops, the locks were being not being unlocked in the order they were locked.
// They were also locked and unlocked in different orders in both for loops. I changed them
// to lock and unlock in the same order for both to prevent the deadlock.

// For the deadlock to occur, in the for loop, one lock (say lock_b) locks, and then lock_a locks.
// Once the comde is complete, lock_b unlocks, but it cannot do so because lock_a must first unlock.
// This causes a deadlock to occur.