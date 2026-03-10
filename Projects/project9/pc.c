#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdbool.h>
#include "eventbuf.h"

int PRO_COUNT;
int CON_COUNT;
int EVENT_COUNT;
int OUT_EVENTS;

sem_t *mutex;
sem_t *items;
sem_t *spaces;

struct eventbuf *eb;
bool finished = false;


sem_t *sem_open_temp(const char *name, int value){
    sem_t *sem;

    // Create the semaphore
    if ((sem = sem_open(name, O_CREAT, 0600, value)) == SEM_FAILED)
        return SEM_FAILED;

    // Unlink it so it will go away after this process exits
    if (sem_unlink(name) == -1) {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}

void usage(){
    puts("usage: ./pc int int int int");
    exit(1);
}

void lock(){
    sem_wait(mutex);
}

void unlock(){
    sem_post(mutex);
}


void *producer_run(void *args){
    int thread_id = *(int*)args;
    (void)thread_id;
    int event;

    // Produce Event
    for (int i = 0; i < EVENT_COUNT; i++){
        event = thread_id * 100 + i; // event number formula

        sem_wait(spaces);
        lock();

        eventbuf_add(eb, event);
        printf("P%d: added event %d\n", thread_id, event);

        unlock();
        sem_post(items);
    }


    printf("P%d: exiting\n", thread_id);
    return NULL;
}


void *consumer_run(void *args){
    int thread_id = *(int*)args;
    (void)thread_id;
    int event;

    // Consume Event

    while(true){
        sem_wait(items);
        lock();

        if (finished && eventbuf_empty(eb)){
            unlock();
            break;
        }

        if (!eventbuf_empty(eb)){
            event = eventbuf_get(eb);
            printf("C%d: got event %d\n", thread_id, event);
        }

        unlock();
        sem_post(spaces);
    }

    printf("C%d: exiting\n", thread_id);
    return NULL;
}



int main(int argc, char* argv[]){

    // Parse Command Line
    if (argc != 5)
        usage();

    PRO_COUNT = atoi(argv[1]);
    CON_COUNT = atoi(argv[2]);
    EVENT_COUNT = atoi(argv[3]);
    OUT_EVENTS = atoi(argv[4]);



    eb = eventbuf_create();

    pthread_t producer[PRO_COUNT];
    int pro_id[PRO_COUNT];

    pthread_t consumer[CON_COUNT];
    int con_id[CON_COUNT];

    mutex = sem_open_temp("mutex", 1);
    items = sem_open_temp("items", 0);
    spaces = sem_open_temp("spaces", OUT_EVENTS);



    // Create Producers and Consumers
    for (int i = 0; i < PRO_COUNT; i++) {
        pro_id[i] = i;
        pthread_create(producer + i, NULL, producer_run, pro_id + i);
    }

    for (int i = 0; i < CON_COUNT; i++) {
        con_id[i] = i;
        pthread_create(consumer + i, NULL, consumer_run, con_id + i);
    }


    // Wait for Producers to exit
    for (int i = 0; i < PRO_COUNT; i++)
        pthread_join(producer[i], NULL);


    // Tell Consumers to exit
    finished = true;
    for (int i = 0; i < CON_COUNT; i++)
        sem_post(items);
    
    for (int i = 0; i < CON_COUNT; i++)
        pthread_join(consumer[i], NULL);


    // Cleanup
    eventbuf_free(eb);
    sem_close(mutex);
    sem_close(items);
    sem_close(spaces);
}