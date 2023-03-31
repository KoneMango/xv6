#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <sys/types.h>
#include "kernel/fcntl.h"

void sieve(int *arr, int index, int size) {
    int prime = arr[index];
    int i, j = 0;
    for (i = 0; i < size; i++) {
        if (arr[i] % prime != 0 || arr[i] == prime) {
            arr[j++] = arr[i];
        }
    }
    for (; j < size; j++) {
        arr[j] = 0;
    }
}

int main(int argc, char* argv[]) {
    int arr[34];
    // 初始化数组
    for (int i = 2; i < 36; i++) {
        arr[i - 2] = i;
    }

    int fd[2];
    int index = 0;
    pid_t pid;

    while (arr[index] != 0 && arr[index] * arr[index] <= arr[sizeof(arr) / sizeof(arr[0]) - 1]) {
        pipe(fd);
        pid = fork();

        if (pid == 0) { // child process(read)
            close(fd[1]);
            read(fd[0], arr, sizeof(arr));
            sieve(arr, index, sizeof(arr) / sizeof(arr[0]));
            index++;
            close(fd[0]);
            pipe(fd);
            pid = fork();
            if (pid != 0) {
                wait(0);
                break;
            }
        } else { // parent process(write)
            close(fd[0]);
            write(fd[1], arr, sizeof(arr));
            close(fd[1]);
            wait(0);
        }
    }

    if (pid != 0) {
        // 输出质数
        for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
            if (arr[i] != 0) {
                printf("prime %d\n", arr[i]);
            }
        }
    }
    exit(0);
}