#include <stdio.h>   // For perror()
#include <fcntl.h>   // For open()
#include <unistd.h>  // For write() and close()

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