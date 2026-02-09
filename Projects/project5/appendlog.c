#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define REC_SIZE 32 // bytes
#define REC_COUNT (1024*1024)
#define HEADER_SIZE (sizeof(int)*2)
#define DATA_SIZE (HEADER_SIZE + REC_SIZE * REC_COUNT)


// ------------Structs------------


// Pair
typedef struct {
    int count;
    char *prefix;
} Pair;

// Lock
struct flock lock = {
    .l_type=F_WRLCK,
    .l_whence=SEEK_SET,
};

// Unlock
struct flock unlock = {
    .l_type=F_UNLCK,  // Set to unlock
    .l_whence=SEEK_SET,
};


// ------------Methods------------


void usage(){
    printf("usage: appendlog int 'string'...\n");
    exit(1);
}

void parse_cl(int argc, char *argv[], Pair *pairs){
    for (int i = 1, j = 0; i < argc; i += 2, j++){
        pairs[j].count = atoi(argv[i]);
        pairs[j].prefix = argv[i+1];
    }
}

void append_record(int fd, void *data, Pair pair){

    for (int i = 0; i < pair.count; i++){

        // --------Handle Header--------
        lock.l_start=0;
        lock.l_len=HEADER_SIZE;
        fcntl(fd, F_SETLKW, &lock); // Lock Header

        int *head_count = data;
        char *head_offset = data + sizeof(int);

        int cur_offset = *head_offset;
        (*head_count)++;
        (*head_offset)++;

        unlock.l_start=0;
        unlock.l_len=HEADER_SIZE;
        fcntl(fd, F_SETLKW, &unlock); // Unlock Header

        // --------Handle Record--------
        char *record_ptr = data + HEADER_SIZE + cur_offset * REC_SIZE;

        lock.l_start=HEADER_SIZE + cur_offset * REC_SIZE;
        lock.l_len=REC_SIZE;
        fcntl(fd, F_SETLKW, &lock); // Lock Record

        int  *rec_count  = (int*)record_ptr;
        char *rec_prefix = record_ptr + sizeof(int);

        *rec_count = i;
        strcpy(rec_prefix, pair.prefix);

        unlock.l_start=HEADER_SIZE + cur_offset * REC_SIZE;
        unlock.l_len=REC_SIZE;
        fcntl(fd, F_SETLKW, &unlock); // Unlock Record
    }
}

void dump_log(int fd, void *data){

    lock.l_start=0;
    lock.l_len=DATA_SIZE;
    fcntl(fd, F_SETLKW, &lock); // Lock File

    int *header_count = data;
    int count = *header_count;

    for (int i = 0; i < count; i++){
        char *record_ptr = data + HEADER_SIZE + i * REC_SIZE;
        int  *rec_count  = (int*)record_ptr;
        char *rec_prefix = record_ptr + sizeof(int);

        printf("%d: %s %d\n", i, rec_prefix, *rec_count);
    }

    unlock.l_start=0;
    unlock.l_len=DATA_SIZE;
    fcntl(fd, F_SETLKW, &unlock); // Unlock File

}

int main(int argc, char *argv[]){

    // Ensure arguements are in pairs
    if (argc % 2 == 0 || argc < 3){
        usage();
    }

    // Setup
    int fd = open("log.dat", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    ftruncate(fd, DATA_SIZE);

    void *data = mmap(NULL, DATA_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == NULL) {
        perror("mmap");
        exit(1);
    }

    // Parse Command Line
    int pairs_amount = (argc-1)/2;
    Pair pairs[pairs_amount];
    parse_cl(argc, argv, pairs);

    // Append Each Record
    for (int i = 0; i < pairs_amount; i++){
        if(fork() == 0){
            Pair cur_pair = pairs[i];
            append_record(fd, data, cur_pair);
            exit(1);
        } 
    }

    // Parent process waits for all the Children
    for (int i = 0; i < pairs_amount; i++){wait(NULL);}

    // Dump the Log
    dump_log(fd, data);

    // Clean-Up
    munmap(data, DATA_SIZE);
    close(fd);
}