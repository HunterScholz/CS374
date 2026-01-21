#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    (void)argc;
    char *token;
    int tnum = 0;

    if ((token = strtok(argv[1], " ")) != NULL) do
        printf("%d: %s\n", tnum++, token);
    while ((token = strtok(NULL, " ")) != NULL);
}