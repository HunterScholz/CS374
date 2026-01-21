#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    char *tokens[128] = {0};
    char *token;
    int tnum = 0;
    char line[1024];

    printf("prompt> ");
    fflush(stdout);
    fgets(line, sizeof line, stdin);

    if ((token = strtok(line, " ")) != NULL) do
        tokens[tnum++] = token;
    while ((token = strtok(NULL, " \n")) != NULL);

    execvp(tokens[0], tokens);
}

// If I type jibberish into the input or something that isn't a command,
// nothing happens and the process terminates. To make such an input call
// and error, you can use perror() around the execvp() call.