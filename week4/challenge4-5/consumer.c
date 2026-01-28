#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define FIFO_PATH "/home/hunter/myfifo"
#define BYTE_LIMIT 255

int main (void){
    printf("waiting for writers...\n");

    char buf[BYTE_LIMIT];
    int fd = open(FIFO_PATH, O_RDONLY);
    ssize_t byte_read;

    while((byte_read = read(fd, buf, BYTE_LIMIT)) > 0){
        printf("%d: %s\n", buf[0], buf+1);
    }

    close(fd);
    exit(0);
}
