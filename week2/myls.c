#include <dirent.h>
#include <stdio.h>

int main(void){
    struct dirent *de;

    DIR *d = opendir("/");
    if (d== NULL){
        perror("opendir");
        return 1;
    }

    while ((de = readdir(d)) != NULL){
        de = readdir(d);
        printf("%s\n", de->d_name);
    }

    closedir(d);
    return 1;
}

