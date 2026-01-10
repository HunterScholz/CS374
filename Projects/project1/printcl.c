#include <stdio.h>

int main(int argc, char *argv[]) {

    printf("%%");

    for(int i = 0; i < argc; i++){
        printf(" %s", argv[i]);
    }

    printf("\n");

    for(int i = 0; i < argc; i++){
        printf("%d: %s\n", i, argv[i]);
    }
}