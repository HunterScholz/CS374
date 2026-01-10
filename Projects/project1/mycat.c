#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd;
    char buf[2048];

    if (argc == 1){
        int bytes_read = read(0, buf, 2048);
        write(1, buf, bytes_read);
    }

    for(int i = 1; i <= argc-1; i++){
        fd = open(argv[i], 0600);

        if (fd == -1) {
            perror("open");
            return 1;  // Exit with error code 1
        }

        int bytes_read = read(fd, buf, 2048);

        write(1, buf, bytes_read);
        printf("\n");

        close(fd);
    }
}
