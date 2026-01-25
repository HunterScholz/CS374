#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void run_command(char *tokens[]){

    pid_t pid = fork();

    if (pid == 0){ // Child
        execvp(tokens[0], tokens);
        perror("execpv failed");
        exit(1);
    } else { // Parent
        wait(NULL);
    }
}

void handle_command(char *tokens[]){
    
    // Create New Line
    if (tokens[0] == NULL){
        return;
    }

    // Handle CD
    if (strcmp(tokens[0], "cd") == 0){
        if (chdir(tokens[1]) == -1){
            perror(tokens[1]);
        }
    } 

    // Exit
    else if (strcmp(tokens[0], "exit") == 0) {
        exit(0);
    } 
    
    // Run Command
    else {
        run_command(tokens);
    }
}


int main(void){
    while (1==1) {
        char *tokens[128] = {0};
        char *token;
        int tnum = 0;
        char line[2048];

        printf("mush> ");
        fflush(stdout);
        fgets(line, sizeof line, stdin);

        if ((token = strtok(line, " \n")) != NULL) do
            tokens[tnum++] = token;
        while ((token = strtok(NULL, " \n")) != NULL);

        handle_command(tokens);
    }
}
