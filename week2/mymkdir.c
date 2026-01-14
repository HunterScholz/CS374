#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* args[]){

    if (argc != 2){
        usage();
    } else if (mkdir(args[1], 0777) == -1){ //If mdir() returns an error, print the error
        perror(args[1]);
        return 1;
    }
}

void usage(void)
{
    fprintf(stderr, "usage: mymkdir dir\n");
    exit(1);
}
