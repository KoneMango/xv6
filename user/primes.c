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
        //读取管道里的数组
        read(fd[0][0], arr, 100);
        close(fd[0][0]);
        close(fd[0][1]);
        for (size_t i = 0; i < sizeof(arr) / sizeof(arr[0])-5; i++)
        {
            printf("prime %d\n", arr[i]);
        }
        
        exit(0);
    } 
    else  // parent process(write)
    {
        for (size_t i = 0; i < sizeof(arr) / sizeof(arr[0])-5; i++)
        {
            if (arr[i] % arr[0] == 0)
            {
                //在数组中去除arr[i]
                for (size_t j = i; j < sizeof(arr) / sizeof(arr[0])-5; j++)
                {
                    arr[j] = arr[j + 1];
                }
            }
        }
        write(fd[0][1], arr, 100);
        close(fd[0][1]);
        close(fd[0][0]);
        wait(0);
        exit(0);
    }
}