#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

long directory_size(char* path){
    DIR *dir = opendir(path);
    struct dirent *entry;
    struct stat buf;
    long total_size = 0;
    char full_path[1024];

    if (dir == NULL){
        perror("opendir");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL){ 
        //Ignore current and parent directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {continue;}

        // Get full path
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &buf) == -1){
            perror("stat");
            continue;
        }

        // Recurse if directory
        if (S_ISDIR(buf.st_mode)){
            total_size += directory_size(full_path);
        } else {
            total_size += buf.st_size;
        }
    }

    printf("%s: %ld\n", path, total_size);
    closedir(dir);
    return total_size;
}

void usage()
{
    fprintf(stderr, "usage: ./filescanner dir\n");
    exit(1);
}

int main(int argc, char *args[]){
    if (argc > 2){usage();}

    // Sets root to current directory if no arguments are given
    char *root = (argc == 2) ? args[1] : ".";

    directory_size(root);
}