#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define FIFO_PATH "/home/hunter/myfifo"
#define BYTE_LIMIT 1024

int main (void){
    char buffer[BYTE_LIMIT];
    int fd = open(FIFO_PATH, O_RDONLY);
    ssize_t byte_read;

    while((byte_read = read(fd, buffer, BYTE_LIMIT)) > 0){
        write(1, buffer, byte_read);
        printf("\n");
    }

    close(fd);
    exit(0);
}

// The consumer reads the FIFO pipe and writes it to the standard output. If there is nothing
// in the FIFO pipe, then the process is stalled.