#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <sys/types.h>
#include "kernel/fcntl.h"

int main(int argc, char* argv[]){
    pid_t pid;
    int fd[2];
    pipe(fd);
    pid = fork();
    int index = 0; 


    int arr[34];
    // 初始化数组
    for (int i = 2; i < 35; i++) 
    {
        arr[i-2] = i;
    }

    pid=fork();
    if (pid == 0) // child process(read)
    {
        close(fd[1]);
        read(fd[0], arr, sizeof(arr));
        for (size_t i = 1 ; i < sizeof(arr) / sizeof(arr[0]); i++)
        {
            if(arr[i]%arr[index]==0)
            {
                arr[i]=arr[i+1];
            }
        }
        printf("%d", arr[index]);
        index ++;
        close(fd[0]);
        exit(0);
    } 
    else  // parent process(write)
    {
        close(fd[0]);
        write(fd[1], arr, sizeof(arr));
        printf("%d", arr);
        close(fd[1]);
        pid = fork();
        wait(0);
        exit(0);
    }
}