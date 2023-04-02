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

void printPrime(int *input, int count)
{
  int p[2], i = 0;
  int prime = input[0];
  pipe(p);
  printf("prime %d\n", prime);

  while (1) {
    if (fork() == 0) {
      close(p[0]);
      for (; i < count; i++) {
        write(p[1], (char *)(input + i), 4);
      }
      close(p[1]);
      exit(0);
    } else {
      close(p[1]);
      count = 0;
      int temp;
      while (read(p[0], (char *)&temp, 4) == 4) {
        if (temp % prime) {
          input[count++] = temp;
        }
      }
      if (count == 0) {
        break;
      }
      for (i = 0; i < count; i++) {
        printf("%d ", input[i]);
      }
      printf("\n");
      prime = input[0];
      i = 1;
    }
    close(p[0]);
    wait(0);
  }
}

int main(int argc, char *argv[]) {
  int input[34], i = 0;
  for (; i < 34; i++) {
    input[i] = i + 2;
  }
  printPrime(input, 34);
  exit(0);
}