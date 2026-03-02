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

void *run1(void *arg) {
    (void)arg;

    while (count != 3){
        lock();
        pthread_cond_wait(&condition, &mutex);
        unlock();
    }

    puts("Halfway there!");
    return NULL;
}

void *run2(void *arg){
    (void)arg;

    while (count != 0){
        lock();
        pthread_cond_wait(&condition, &mutex);
        unlock();
    }
    
    puts("Liftoff!");
    return NULL;
}

int main(void){
    pthread_t t1;
    pthread_t t2;

    count = 6;

    pthread_create(&t1, NULL, run1, NULL);
    pthread_create(&t2, NULL, run2, NULL);

    while (count > 0){
        lock();
        count--;
        printf("%d\n", count);
        pthread_cond_broadcast(&condition);
        unlock();
        sleep(1);
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

// If you use pthread_con_signal instead of the broadcast, a random thread is selected. With multiple threads,
// A thread with a condition that has been met could potentially not be called, causing that thread not to
// wake up and run when it should.