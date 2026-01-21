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