#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_COUNT 10

sem_t *sem;
sem_t *mutex;
int count;

sem_t *sem_open_temp(const char *name, unsigned int value)
{
    sem_t *sem;

    // Create the semaphore
    if ((sem = sem_open(name, O_CREAT, 0666, value)) == SEM_FAILED)
        return SEM_FAILED;

    // Unlink it so it will go away after this process exits
    if (sem_unlink(name) == -1) {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}

void *run(void *args)
{
    int thread_id = *(int*)args;

    (void)thread_id;  // quiet unused variable warning

    sem_wait(mutex);
    count++;
    sem_post(mutex);
    if (count == THREAD_COUNT){
        printf("Thread %d: Opening turnstile\n", thread_id);
        sem_post(sem);
        return NULL;
    }
    printf("Thread %d: Waiting for turnstile\n", thread_id);
    sem_wait(sem);
    printf("Thread %d: In turnstile\n", thread_id);
    count--;
    printf("Thread %d: Reopening turnstile\n", thread_id);
    sem_post(sem);

    return NULL;
}

int main(void)
{
    pthread_t thread[THREAD_COUNT];
    int thread_id[THREAD_COUNT];

    sem = sem_open_temp("sem", 0);
    mutex = sem_open_temp("mutex", 1);

    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_id[i] = i;
        pthread_create(thread + i, NULL, run, thread_id + i);
    }

    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_join(thread[i], NULL);

    sem_close(sem);
    sem_close(mutex);
}

// You would have to wait for the semaphore with a different thread to close it again. Since all the threads are
// finished and went through, the turnstile is left open with nothing left to wait for it.
