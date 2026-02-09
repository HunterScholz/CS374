#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

void sigint_handler(int sig)
{
    const char msg[] = "\nI got SIGINT!\n";
    write(1, msg, sizeof(msg));
    (void)sig;
}

int main(void){

    struct sigaction sa = {
        .sa_handler = sigint_handler,
        .sa_flags = 0,
    };

    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    puts("Program will exit in 10 seconds. Hit ^C!");
    int i = sleep(10);
    printf("%d\n", i);

}

//1. When I hit ^C the program exited immediantly, but still printed the message.

//2. The return value for sleep() is how many seconds are left in the sleep() call.