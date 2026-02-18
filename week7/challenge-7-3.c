#include <stdio.h>
#include <pthread.h>

#define THREAD_COUNT 10

void *run (void* args){
    int *id = args;
    printf("thread %d running\n", *id);
    return NULL;
}

int main (void){
    pthread_t thread[THREAD_COUNT];
    int thread_id[THREAD_COUNT];
    
    // Create Threads
    for (int i = 0; i < THREAD_COUNT; i++){
        thread_id[i] = i;
        pthread_create(thread+i, NULL, run, thread_id+i);
    }

    // Join Threads
    for (int i = 0; i < THREAD_COUNT; i++){
        pthread_join(thread[i], NULL);
    }

    puts("main thread complete");

}