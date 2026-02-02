#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

#define DATA_SIZE 1024

void usage(){
    printf("usage: mmap_write int float 'string'\n");
}

int main (int argc, char *argv[]){
    if (argc != 4){
        usage();
        exit(1);
    }

    int fd = open("data.dat", O_RDWR);
    if (fd == -1){
        perror("error opening file");
        exit(1);
    }

    void *data = mmap(NULL, DATA_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    // Arguements    
    int i = atoi(argv[1]);
    float f = atof(argv[2]);
    char *s = argv[3];

    // Pointers
    int *ip = data;
    float *fp = data + sizeof(int);
    char *sp = data + sizeof(int) + sizeof(float);

    // Assign Data to the Pointers
    *ip = i;
    *fp = f;
    strcpy(sp, s);

    // Confirmation
    printf("Wrote: %d %f %s\n", i, f, s);

    // Cleanup
    munmap(data, DATA_SIZE);
    close(fd);
}