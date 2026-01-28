#include <unistd.h>
#include <string.h>

int main (void){
    int pfds[2];
    pipe(pfds);

    char buffer[1024];
    char* s = "Hello\n";
    int s_bytes = strlen(s);

    write(pfds[1], s, s_bytes);
    read(pfds[0], buffer, s_bytes);

    write(1, buffer, s_bytes);
}