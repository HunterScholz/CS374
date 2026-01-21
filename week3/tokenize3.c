#include <stdio.h>
#include <string.h>

int main(void)
{
    char *tokens[128] = {0};
    char *token;
    int tnum = 0;
    char line[1024];
    fgets(line, sizeof line, stdin);

    if ((token = strtok(line, " ")) != NULL) do
        tokens[tnum++] = token;
    while ((token = strtok(NULL, " \n")) != NULL);

    for (int i = 0; tokens[i] != NULL; i++)
        printf("%d: %s\n", i, tokens[i]);
}

"""
The for loop could fail since the array is full of content unrelated to the
program. This garbage data could muddy the process, so it should be initialized
with only NULL pointers.
"""