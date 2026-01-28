#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define FIFO_PATH "/home/hunter/myfifo"

int main (int argc, char* argv[]){

    int count = atoi(argv[1]);
    if (count < 1 || count > 255){
        printf("cound out of range");
        exit(1);
    }

    printf("waiting for readers...\n");
    
    mkfifo(FIFO_PATH, 0666);
    int fd = open(FIFO_PATH, O_WRONLY);

    for (int i = 0; i <= count; i++){
        char buf[16];

        buf[0] = i;
        sprintf(buf + 1, "Message #%d", i);
        printf("Sending %d %s\n", i, buf+1);

        sleep(1);
        write(fd, buf, 16);
    }

    close(fd);
    exit(0);
}
