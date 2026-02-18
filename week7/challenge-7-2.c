#include <stdio.h>
#include <pthread.h>

struct Parameters {
        char *hello;
        char *goodbye;
    };

void *run (void *args){
    struct Parameters *params = (struct Parameters *)args;

    printf("child: %s\n", params->hello);
    printf("child: %s\n", params->goodbye);

    return args;
}

int main (void){
    pthread_t t1;

    struct Parameters *params;
    params->hello = "my name is yuven marasini";
    params->goodbye = "i'm four years old!";
    
    puts("parent: hello!");

    pthread_create(&t1, NULL, run, (void *)params);
    pthread_join(t1, NULL);

    puts("parent: goodbye!");
}