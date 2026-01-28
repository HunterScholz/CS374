#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main (void){
    int fd = open("output.txt", O_CREAT|O_TRUNC|O_WRONLY, 0644);
    if (fd == -1) {
        perror("open");
        exit(1);
    } 

    dup2(fd, 1);

    printf("Hello, world\nThis is a test!\n");
    fflush(stdout);
    close(fd);
}

// To save the old stdout, you could dup() it before closing. When 1 is closed, you can reopen it by
// reduping the stdout back onto 1, returning it to displaying on the screen.
