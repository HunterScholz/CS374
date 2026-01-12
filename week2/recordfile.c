#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define RECORD_SIZE 32  // bytes

void seek(int fd, int record_num){
    lseek(fd, record_num*RECORD_SIZE, SEEK_SET);
}

/**
 * Read the record into the buffer
 */
void read_record(int fd, int record_num, char *buffer)
{
    seek(fd, record_num);
    read(fd, buffer, RECORD_SIZE);
}

/**
 * Read the record from the buffer
 */
void write_record(int fd, int record_num, char *buffer)
{
    seek(fd, record_num);
    write(fd, buffer, RECORD_SIZE);
}

///////////////////////////////////////////////////////////
// Understand but don't modify the code below this point //
///////////////////////////////////////////////////////////

/**
 * Different app modes
 */
enum mode {
    MODE_READ,
    MODE_WRITE
};

/**
 * App context
 */
struct context {
    enum mode mode;
    int rec_num;
    char *buf;
};

/**
 * Print a usage message and exit
 */
void usage(void)
{
    fprintf(stderr, "usage: recordfile read rec_num | write rec_num data\n");
    exit(1);
}

/**
 * Parse command line.
 */
void parse_command_line(int argc, char *argv[], struct context *context)
{
    if (argc < 2) usage();

    if (argv[1][0] == 'r') {
        if (argc != 3) usage();

        context->mode = MODE_READ;
        context->rec_num = atoi(argv[2]);

    } else if (argv[1][0] == 'w') {
        if (argc != 4) usage();

        context->mode = MODE_WRITE;
        context->rec_num = atoi(argv[2]);
        strncpy(context->buf, argv[3], RECORD_SIZE);
        context->buf[RECORD_SIZE-1] = '\0';

    } else
        usage();
}

/**
 * Main
 */
int main(int argc, char *argv[])
{
    char buf[RECORD_SIZE];
    struct context context = { .buf = buf };

    parse_command_line(argc, argv, &context);

    // Open the file
    int fd;

    if ((fd = open("recordfile.dat", O_RDWR|O_CREAT, 0600)) == -1) {
        perror("open");
        return 2;
    }

    // Read or write records
    switch (context.mode) {
        case MODE_READ:
            read_record(fd, context.rec_num, buf);
            printf("%d: %s\n", context.rec_num, buf);
            break;

        case MODE_WRITE:
            write_record(fd, context.rec_num, context.buf);
            break;
    }

    close(fd);
}