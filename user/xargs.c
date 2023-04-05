#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define MAXARG 32    //最大参数个数
#define MAXARGLEN 64 //最大参数长度

int main(int argc, char *argv[]) {
    char *args[MAXARG]; //参数列表
    char buf[MAXARGLEN]; //缓冲区
    int nargs = 0; //参数个数
    int i, n;

    if (argc < 2) { //参数错误
        fprintf(2, "usage: xargs cmd [args ...]\n");
        exit(1);
    }

    //读取标准输入，并将每个字符串作为参数传递给指定命令
    while ((n = read(0, buf, MAXARGLEN)) > 0) { //从标准输入读取数据
        for (i = 0; i < n; i++) { //遍历缓冲区中的每个字符
            if (buf[i] == '\n') { //如果遇到换行符，则将参数列表传递给指定命令
                args[nargs] = 0; //最后一个参数必须为NULL

                //子进程操作
                if (fork() == 0) { //创建子进程，执行指定命令，并传递参数
                    exec(argv[1], args); //执行指定命令，并传递参数
                    fprintf(2, "xargs: exec %s failed\n", argv[1]); //指定命令执行失败
                    exit(1);
                } 

                //父进程操作
                else {
                    wait(0); //等待子进程结束
                }
                nargs = 0; //清空参数列表，以便下一次使用
            } 
        
            else if (isspace(buf[i])) { //如果遇到空格或制表符，则将它们转换为NULL字符，作为参数分隔符
                buf[i] = 0;
            } else {
                args[nargs++] = &buf[i]; //将参数添加到参数列表中
                while (!isspace(buf[i]) && i < n) i++; //跳过单词，找到下一个分隔符
                i--; //让下次循环能继续读取这个单词的字符
            }
            if (nargs >= MAXARG) { //参数个数超过最大限制
                fprintf(2, "xargs: too many arguments\n");
                exit(1);
            }
        }
    }

    exit(0);
}
