#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

void sigusr1_handler(int sig)
{
    (void)sig;
    write(1, "SIGUSR1 handled\n", 16);
}

int main(void){

    struct sigaction sa = {
        .sa_handler = sigusr1_handler,
        .sa_flags = SA_RESTART,
    };

    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("Reading from the keyboard. But don't type anything.\n");
    printf("Go to another window and `kill -USR1 %d`\n", getpid());

    char *buf[128];
    if (read(0, buf, 128) == -1){
        perror("read");
    }

}

//1. When the singal was sent the process ended with an error and the SIGUSR1 was handled.

//2. The signal does not end the process with the SA_RESTART flag, though it repeatedly says that 
// SIGUSR1 was handled when its called repeatedly.