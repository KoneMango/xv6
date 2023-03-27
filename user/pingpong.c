#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <sys/types.h>
#include "kernel/fcntl.h"

int main(int argc, char* argv[])
{
    pid_t pid;
    char buffer[20];
    int fd1[2];
    pipe(fd1);
    pid = fork();

    if (pid == 0) // child process(read)
    {
        close(fd1[1]);
        read(fd1[0], buffer, sizeof(buffer));
        close(fd1[0]);
        printf("%s", buffer);
        exit(0);
    } else  // parent process(write)
    {
        close(fd1[0]);
        write(fd1[1], "h", 1);
        close(fd1[1]);
        wait(0);
        exit(0);
    }
    return 0;
}