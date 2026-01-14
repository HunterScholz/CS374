#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


// Get File Size
int main(int argc, char *args[]){

    if (argc < 2) {
        // ERROR if there are too many arguments
        perror("too many arguments");
        return 1;  // Exit with error code 1
    }

    for(int i = 1; i < argc; i++){
        struct stat buf;
        stat(args[i], &buf);

        printf("%s: %d\n", args[i], buf.st_size); 
    }
    
}

// Open and Read File
int main(void)
{
    // Integer to hold the file descriptior
    int fd;
    fd = open("foo.txt", 0600);

    if (fd == -1) {
        // If the open fails, print out an error message prepended with
        // "open"
        perror("open");
        return 1;  // Exit with error code 1
    }

    char buf[128];  // 128-byte buffer
    int bytes_read = read(fd, buf, 128);

    // Write to stdout
    write(1, buf, bytes_read);

    // Close the file when done
    close(fd);
}