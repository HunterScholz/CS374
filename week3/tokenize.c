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

"""
We pass a pointer to the first strtok() because the first call tells strtok() where to look,
and subsequent calls of it still only return the first element. The next calls need to call NULL
since strtok() is already looking at the string.
"""