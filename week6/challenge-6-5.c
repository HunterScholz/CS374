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

    sigset_t mask, oldmask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    sigprocmask(SIG_BLOCK, &mask, &oldmask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("SIGUSR1 is blocked.\n");
    printf("Reading from the keyboard. But don't type anything.\n");
    printf("Go to another window and `kill -USR1 %d`\n", getpid());
    printf("Then press RETURN to continue.\n");

    char *buf[128];
    if (read(0, buf, 128) == -1){
        perror("read");
    }

    sigprocmask(SIG_UNBLOCK, &mask, &oldmask);

    printf("Signal unblocked.\n");

}

// 1. When several of the same signal are deferred, they are combined together into a single signal.
