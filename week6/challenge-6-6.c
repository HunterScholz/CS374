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
        .sa_flags = SA_RESTART,
    };

    sigemptyset(&sa.sa_mask);

    sigset_t mask, oldmask, zero;

    sigemptyset(&zero);
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    sigprocmask(SIG_BLOCK, &mask, &oldmask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    fd_set readfds;

    FD_ZERO(&readfds);   // First clear it
    FD_SET(0, &readfds); // Set fd 0

    printf("Either enter some text, or...\n");
    printf("go to another window and `kill -USR1 %d`.\n", getpid());

    int psel = pselect(1, &readfds, NULL, NULL, NULL, &zero);
    if (psel == -1 && errno == EINTR && sigusr1_happened){
        printf("Got USR1\n");
    } 
    
    else if (psel > 0){
        if (FD_ISSET(0, &readfds)){
            char *buf[128];
            size_t size = read(0, buf, sizeof(buf));
            write(1, buf, size);
        }
    }

    sigprocmask(SIG_UNBLOCK, &mask, &oldmask);
    printf("Signal unblocked\n");

}
