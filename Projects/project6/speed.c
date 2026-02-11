#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

volatile sig_atomic_t sigusr1_happened;

void sigusr1_handler(int sig)
{
    sigusr1_happened = true;
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

    while (sigusr1_happened){
        sleep(10);
    }

    char *buf[128];
    if (read(0, buf, 128) == -1){
        perror("read");
    }
}
