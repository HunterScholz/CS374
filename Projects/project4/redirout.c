#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main (int argc, char *argv[]){
    if (argc <= 3){
        printf("usage: redirout filename command [arg1 [arg2 ... ] ]\n");
        exit(1);
    }

    char *fout = argv[1];
    int fd = open(fout, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(1);
    } 

    dup2(fd, 1);

    char **point = &argv[2];
    execvp(argv[2], point);
    fflush(stdout);
    close(fd);
}
