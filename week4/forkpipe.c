#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main (void){
    int pfds[2];
    pipe(pfds);

    char buffer[1024];
    char* s = "Hello, parent!\n";
    int s_bytes = strlen(s);

    pid_t pid = fork();

    if (pid == 0){
        close(pfds[0]);
        write(pfds[1], s, s_bytes);
        exit(1);
    } else {
        close(pfds[1]);
        read(pfds[0], buffer, s_bytes);
        write(1, buffer, s_bytes);
        wait(NULL);
    }
}