#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char *args[]){

    for(int i = 1; i < argc; i++){
        struct stat buf;
        stat(args[i], &buf);

        printf("%s: %d\n", args[i], buf.st_size); 
    }
    
}