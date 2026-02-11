#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

volatile sig_atomic_t sigusr1_happened;

void sigusr1_handler(int sig)
{
    sigusr1_happened = 1;
    (void)sig;
    write(1, "SIGUSR1 handled\n", 16);
}

int main(void){

    struct sigaction sa = {
        .sa_handler = sigusr1_handler,
        .sa_flags = 0,
    };

    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("Go to another window and `kill -USR1 %d`\n", getpid());

    while (sigusr1_happened == 1){
        sleep(10);
    }

    char *buf[128];
    if (read(0, buf, 128) == -1){
        perror("read");
    }
}

// 1. If you continually check in a loop, than the code is spinning, and it uses your entire CPU which is bad for
// your computer.

// 2. The volatile keyword lets the program know not to optomize the variable because it could change at any time.
// It is needed with sig_atomic_t because the number updates during runtime within a signal handler, and volatile
// prevents it from becoming desynced.