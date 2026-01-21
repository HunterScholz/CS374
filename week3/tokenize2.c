#include <stdio.h>
#include <string.h>

int main(void)
{
    char *token;
    int tnum = 0;
    char line[1024];
    fgets(line, sizeof line, stdin);

    if ((token = strtok(line, " ")) != NULL) do
        printf("%d: %s\n", tnum++, token);
    while ((token = strtok(NULL, " \n")) != NULL);
}


// Instead of using strtok(), we could find the index of the newline character
// and replace it with NULL so that the string terminates.