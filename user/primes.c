#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <sys/types.h>
#include "kernel/fcntl.h"

int main(int argc, char* argv[]){
    pid_t pid;
    int fd[10][2];
    pid = fork();
    int arr[35];
    // 初始化数组
    for (int i = 2; i < 35; i++) 
    {
        arr[i] = i + 1;
    }
    pid=fork();
    if (pid == 0) // child process(read)
    {
        read(fd[0][0], arr[10], sizeof(arr[10]));
        
        exit(0);
    } 
    else  // parent process(write)
    {
        for (size_t i = 0; i < sizeof(arr); i++)
        {
            if (arr[i] % arr[0] == 0)
            {
                //在数组中去除arr[i]
                for (size_t j = i; j < sizeof(arr); j++)
                {
                    arr[j] = arr[j + 1];
                }
            }
        }
        write(fd[0][1], arr, 100);
        printf(arr);

        wait(0);
        exit(0);
    }




}