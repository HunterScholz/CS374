#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void f1(void){
    puts("f1 called");
}

void f2(void){
    puts("f2 called");
}


int main (void){

    void (*runf[])(void) = {f1, f2, f2, f1, f1, f2, f2, f1, f2, f1, NULL};

    int i = 0;
    while (runf[i] != NULL){
        runf[i]();
        i++;
    }
}