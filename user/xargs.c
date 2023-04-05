#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    int p[2];
    int i, j, n;
    char buf[4];
    int prime;
    int count = 0;
    int arr[MAXARG];

    if (argc < 2) {
        fprintf(2, "Usage: xargs command\n");
        exit(1);
    }

    // 从标准输入读取数据
    while (read(0, buf, 4) > 0) {
        arr[count++] = atoi(buf);
    }

    // 创建管道
    pipe(p);

    // 创建子进程
    if (fork() == 0) {
        // 子进程关闭读端
        close(p[0]);

        // 将当前数组中的所有数写入管道
        for (i = 0; i < count; i++) {
            write(p[1], (char *)(arr + i), 4);
        }

        // 子进程关闭写端，并退出
        close(p[1]);
        exit(0);
    } else {
        // 父进程关闭写端
        close(p[1]);

        // 重置count为0，用于统计剩余未被筛选的数
        count = 0;

        // 从管道中读取数据
        while (read(p[0], buf, 4) > 0) {
            n = atoi(buf);
            if (n % arr[0] != 0) {
                arr[count++] = n;
            }
        }

        // 关闭管道
        close(p[0]);

        // 递归调用printPrime函数
        printPrime(arr, count);
    }

    exit(0);

}