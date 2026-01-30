#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[]){
    char *dir = (argc >= 2) ? argv[1] : ".";

    int pfd[2];
    pipe(pfd);
    int pipe_write = pfd[1];
    int pipe_read = pfd[0];

    pid_t pid = fork();

    // Child
    if (pid == 0){
        dup2(pipe_read, 0);
        close(pipe_write);

        execlp("wc", "wc", "-l", (char *)NULL);
        perror("execlp failed");

        exit(1);
    } 
    
    // Parent
    else {
        dup2(pipe_write, 1);
        close(pipe_read);

        execlp("ls", "ls", "-1a", dir, (char *)NULL);
        perror("execlp failed");

        wait(NULL);
    }
}