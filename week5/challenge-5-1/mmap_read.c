#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

#define DATA_SIZE 1024

int main (void){
    int fd = open("data.dat", O_RDONLY);
    if (fd == -1){
        perror("error opening file");
        exit(1);
    }

    void *data = mmap(NULL, DATA_SIZE, PROT_READ, MAP_SHARED, fd, 0);

    // Pointers
    int *ip = data;
    float *fp = data + sizeof(int);
    char *sp = data + sizeof(int) + sizeof(float);

    // Confirmation
    printf("Read: %d %f %s\n", *ip, *fp, sp);

    // Cleanup
    munmap(data, DATA_SIZE);
    close(fd);
}