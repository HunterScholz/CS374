#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

int MAX_LIMIT = 2147483647;
int MIN_LIMIT = 0;

volatile sig_atomic_t usr1_happened, usr2_happened;
int speed = 0;

//----------- USR1 -----------
void usr1_handler(){
    usr1_happened = 1;
}

struct sigaction sa1 = {
        .sa_handler = usr1_handler,
        .sa_flags = SA_RESTART,
};

//----------- USR2 -----------
void usr2_handler(){
    usr2_happened = 1;
}

struct sigaction sa2 = {
    .sa_handler = usr2_handler,
    .sa_flags = SA_RESTART,
};

void increase_speed(){
    if (speed < MAX_LIMIT){
        speed++;
        printf("increase speed to: %d\n", speed);
    } else {
        printf("You have opened a wormhole to another dimension.\n");
    }
}

void decrease_speed(){
    if (speed > MIN_LIMIT){
        speed--;
        printf("decrease speed to: %d\n", speed);
    } else {
        printf("You cannot go any slower.\n");
    }
}

void handle_input(char *input, int size){
    for (int i = 0; i < size; i++) {
        char command = input[i];

        // Increase
        if (command == '+'){
            increase_speed();
        }

        // Decrease
        else if (command == '-'){
            decrease_speed();
        }

        // Quit Program
        else if (command == 'q'){
            exit(0);
        }
    }
}

int main(void){

    // Print off the PID
    printf("PID = %d\n", getpid());

    // Mask Setup
    sigemptyset(&sa1.sa_mask);
    sigemptyset(&sa2.sa_mask);

    sigset_t mask, oldmask, zero;

    sigemptyset(&zero);
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    sigprocmask(SIG_BLOCK, &mask, &oldmask);

    // Error Handling
    if (sigaction(SIGUSR1, &sa1, NULL) == -1 || sigaction(SIGUSR2, &sa2, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    fd_set readfds;

    FD_ZERO(&readfds);   // First clear it
    FD_SET(0, &readfds); // Set fd 0

    // Pselect Loop
    while(1){
        int psel = pselect(1, &readfds, NULL, NULL, NULL, &zero);

        // Signal Input
        if (psel == -1 && errno == EINTR){
            if (usr1_happened == 1){
                decrease_speed();
                usr1_happened = 0;
            }
            
            else if (usr2_happened == 1){
                increase_speed();
                usr2_happened = 0;
            }
        }

        // Command Line Input
        else if (psel > 0){
            if (FD_ISSET(0, &readfds)){
                char *buf[128];
                size_t size = read(0, buf, sizeof(buf));
                handle_input((char*)buf, size);
            }
        }
    }
}
