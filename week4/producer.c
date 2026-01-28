#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define FIFO_PATH "/home/hunter/myfifo"

int main (int argc, char* argv[]){
    
    mkfifo(FIFO_PATH, 0666);
    int fd = open(FIFO_PATH, O_WRONLY);
    write(fd, argv[1], strlen(argv[1]));

    close(fd);
    exit(0);
}

// The producer writes to the FIFO and then waits for the FIFO to be read, so the process is stalled.