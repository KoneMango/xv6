#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <sys/types.h>
#include "kernel/fcntl.h"

int main(int argc, char* argv[]){
    pid_t pid;
    int fd[10][2];
    pipe(fd[0]);
    pid = fork();

    int arr[34];
    // 初始化数组
    for (int i = 2; i < 35; i++) 
    {
        arr[i-2] = i + 1;
    }

    pid=fork();
    if (pid == 0) // child process(read)
    {
        close(fd[0][1]);
        read(fd[0][0], arr, sizeof(arr));
        close(fd[0][0]);
        for (size_t i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
        {
            printf("prime %d\n", arr[i]);
        }
        exit(0);
    } 
    else  // parent process(write)
    {
        close(fd[0]);
        for (size_t i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
        {
            if (arr[i] != 0)
            {
                int prime = arr[i];
                for (size_t j = i + 1; j < sizeof(arr) / sizeof(arr[0]); j++)
                {
                    if (arr[j] % prime == 0)
                    {
                        arr[j] = 0;
                    }
                }
            }
        }
        write(fd[1], arr, sizeof(arr));
        close(fd[1]);
        wait(0);
        exit(0);
    }
}