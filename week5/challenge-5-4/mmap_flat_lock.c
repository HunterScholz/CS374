#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define REC_SIZE 16 // bytes
#define REC_COUNT (1024*1024)
#define FILE_SIZE (REC_SIZE * REC_COUNT)
#define MAX_COUNT 20000

/**
 * Add the next record to the region.
 */
void add_next_record(int fd, void *data)
{
    // Lock
    struct flock lock = {
        .l_type=F_WRLCK, // Or F_RDLCK for a read lock
        .l_whence=SEEK_SET,
        .l_start=0,
        .l_len=0,
    };

    // Unlock
    struct flock unlock = {
        .l_type=F_UNLCK,  // Set to unlock
        .l_whence=SEEK_SET,
        .l_start=0,
        .l_len=0,
    };


    fcntl(fd, F_SETLKW, &lock);
    
    int *count = data;
    int *loc = data+sizeof(int) * (*count+1);
    *loc = *count;
    (*count)++;

    fcntl(fd, F_SETLKW, &unlock);
}

/**
 * Main.
 */
int main(void)
{
    int fd;

    if ((fd = open("data.dat", O_RDWR|O_CREAT, 0666)) == -1) {
        perror("open");
        return 1;
    }

    ftruncate(fd, FILE_SIZE);

    void *data = mmap(NULL, FILE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    if (data == NULL) {
        perror("mmap");
        return 2;
    }

    int *count = data;
    int *records = count + 1;
    *count = 0;

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 3;
    }

    // Both parent and child are running here

    for (int i = 0; i < MAX_COUNT; i++) {
        add_next_record(fd, data);
    }

    if (pid != 0) {
        // Parent only
        wait(NULL);

        printf("count = %d\n", *count);

        // *2 because both parent and child added MAX_COUNT
        for (int i = 0; i < MAX_COUNT * 2; i++) {
            if (records[i] != i)
                printf("records[%d] == %d\n", i, records[i]);
        }
    }
}

// Without the locks, the record values are getting mixed up because the processes are reading
// over the same count number, which leads to them writing each other over. This mixes up the
// record values, making them slightly off.