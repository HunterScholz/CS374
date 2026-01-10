#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd;
    fd = open(argv[1], 0600);

    if (fd == -1 || argc > 2) {
        perror("open");
        return 1;  // Exit with error code 1
    }

    char buf[2048];
    int bytes_read = read(fd, buf, 2048);

    write(1, buf, bytes_read);
    printf("\n");

    close(fd);
}
