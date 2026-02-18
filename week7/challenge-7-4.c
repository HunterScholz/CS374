#include <stdio.h>
#include <pthread.h>

#define THREAD_COUNT 10
#define ARRAY_SIZE 100000
int array[ARRAY_SIZE] = {0};

void *run (void* args){
    int *array = args;
    
    for (int i = 0; i < ARRAY_SIZE; i++){
        array[i]++;
    }

    return NULL;
}

int main (void){
    pthread_t thread[THREAD_COUNT];
    
    // Create Threads
    for (int i = 0; i < THREAD_COUNT; i++){
        pthread_create(thread+i, NULL, run, &array);
    }

    // Join Threads
    for (int i = 0; i < THREAD_COUNT; i++){
        pthread_join(thread[i], NULL);
    }

    for (int i = 0; i < ARRAY_SIZE; i++){
        if (array[i] != THREAD_COUNT){
            printf("array[%d] = %d, expected %d\n", i, array[i], THREAD_COUNT);
        }
    }
}