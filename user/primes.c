// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"
// #include <sys/types.h>
// #include "kernel/fcntl.h"

// int main(int argc, char* argv[]){
//     pid_t pid;
//     int fd[2];
//     pipe(fd);
//     pid = fork();
//     int index = 0; 


//     int arr[34];
//     // 初始化数组
//     for (int i = 2; i < 35; i++) 
//     {
//         arr[i-2] = i;
//     }

//     while(index < 34)
//     {
//         pid=fork();
//         if (pid == 0) // child process(read)
//         {
//             close(fd[1]);
//             read(fd[0], arr, sizeof(arr));
//             // for (size_t i = 1 ; i < sizeof(arr) / sizeof(arr[0]); i++)
//             // {
//             //     if(arr[i]%arr[index]==0)
//             //     {
//             //         arr[i]=arr[i+1];
//             //     }
//             // }
            
//             //筛选出不是质数的数，并去除
//             for (size_t i = 1 ; i < sizeof(arr) / sizeof(arr[0]); i++)
//             {
//                 if(arr[i]%arr[index]==0)
//                 {
//                     arr[i]=arr[i+1];
//                 }
//             }
//             printf("%d\n", arr[index]);
//             printf("%d\n", arr[index+1]);
//             printf("%d\n", arr[index+2]);
//             printf("%d\n", arr[index+3]);
//             printf("%d\n", arr[index+4]);
//             printf("%d\n", arr[index+5]);
//             printf("%d\n", arr[index+6]);
//             index ++;
//             close(fd[0]);
//             exit(0);
//         } 
//         else  // parent process(write)
//         {
//             close(fd[0]);
//             write(fd[1], arr, sizeof(arr));
//             // printf("%d", arr[0]);
//             close(fd[1]);
//             pid = fork();
//             wait(0);
//             exit(0);
//         }

//     }

// }

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// printPrime函数使用递归方法筛选出素数并打印
void printPrime(int *input, int count)
{
    // 如果count为0，则返回
    if (count == 0) {
        return;
    }

    // 创建管道
    int p[2], i = 0, prime = *input;
    pipe(p);

    // 创建缓冲区，用于从管道中读取数据
    char buff[4];

    // 打印当前素数
    printf("prime %d\n", prime);

    // 创建子进程
    if (fork() == 0) 
    {
        // 子进程关闭读端
        close(p[0]);

        // 将当前数组中的所有数写入管道
        for (; i < count; i++) {
            write(p[1], (char *)(input + i), 4);
        }

        // 子进程关闭写端，并退出
        close(p[1]);
        exit(0);
    } 
    //如果是父进程的话
    else 
    {
        // 父进程关闭写端
        close(p[1]);

        // 重置count为0，用于统计剩余未被筛选的数
        count = 0;

        int *new_input = (int *)malloc(count * sizeof(int)); // 分配新数组的空间
        // 从管道中读取数据，直到管道为空
        while (read(p[0], buff, 4) != 0) {
            int temp = *((int *)buff);


            // 如果当前数不能被素数整除，将其存入数组，并更新count
            if (temp % prime) // 不能被整除(好的，留下)
            {
                new_input[count] = temp;
                new_input++;
                count++;
                // *new_input = temp; // 保存到数组中，把好的数保存到数组中
                // new_input++;
                // count++;
            }
        }
        free(new_input);
        // 递归调用printPrime函数处理剩余的数
        printPrime(new_input - count, count);

        // 父进程关闭读端并等待子进程退出
        close(p[0]);
        wait(0);
    }
}

int main(int argc, char *argv[]) {
    // 创建一个长度为34的数组，用于存放2至35的整数
    int input[34], i = 0;
    for (; i < 34; i++) {
        input[i] = i + 2;
    }

    // 调用printPrime函数处理整数数组
    printPrime(input, 34);
    exit(0);
}