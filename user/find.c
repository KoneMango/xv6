#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


//用来找文件夹里文件的路径
//输入的是文件的全路径，输出的是最后一个文件的文件名
char* fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  buf[strlen(p)] = 0;
  return buf;
}

// void find(char *path , char *fileName)
// {
//   char buf[512], *p;
//   int fd;
//   struct dirent de;
//   struct stat st;

//   if((fd = open(path, 0)) < 0){
//     fprintf(2, "ls: cannot open %s\n", path);
//     return;
//   }

//   if(fstat(fd, &st) < 0){
//     fprintf(2, "ls: cannot stat %s\n", path);
//     close(fd);
//     return;
//   }

//   switch(st.type){
//   case T_FILE:
//     if(strcmp(fmtname(path),fileName) == 0)
//     {
//         printf("%s\n", fileName);
//     }
//     break;

//   case T_DIR:
//   //长度超了
//     if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
//       printf("ls: path too long\n");
//       break;
//     }
//     //把文件名复制到buf里
//     strcpy(buf, path);
//     //把buf的指针指向buf的最后一个字符
//     p = buf+strlen(buf);
//     //把buf的最后一个字符改成/
//     *p++ = '/';
//     //把buf的最后一个字符改成0
//     while(read(fd, &de, sizeof(de)) == sizeof(de)){
//         //如果文件名为空，就跳过
//       if(de.inum == 0)
//         continue;
//         //把文件名复制到buf的最后一个字符后面
//       memmove(p, de.name, DIRSIZ);
//       //把buf的最后一个字符改成0
//       p[DIRSIZ] = 0;
//         //如果文件名是.或者..，就跳过
//       if(stat(buf, &st) < 0){
//         printf("ls: cannot stat %s\n", buf);
//         continue;
//       }
//         //如果文件名是.或者..，就跳过
//        printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
//     }
//     break;
//   }
//   close(fd);
// }

void find(char *path, char *fileName)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0)
  {
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type)
  {
    //如果是文件，就输出文件名
  case T_FILE:
    if (strcmp(fmtname(path), fileName) == 0)
    {
      printf("%s\n", fileName);
    }
    break;
    //如果是文件夹，就递归
  case T_DIR:
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
      printf("ls: path too long\n");
      break;
    }
    //把文件名复制到buf里
    strcpy(buf, path);
    //把buf的指针指向buf的最后一个字符
    p = buf + strlen(buf);
    //把buf的最后一个字符改成/
    *p++ = '/';
    //把buf的最后一个字符改成0
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        //如果文件名为空，就跳过
      if (de.inum == 0)
        continue;
        //把文件名复制到buf的最后一个字符后面
      memmove(p, de.name, DIRSIZ);
      //把buf的最后一个字符改成0
      p[DIRSIZ] = 0;
      //如果文件名是.或者..，就跳过
      if (stat(buf, &st) < 0)
      {
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      if (st.type == T_FILE)
      {
        if (strcmp(fmtname(buf), fileName) == 0)
        {
          printf("%s\n", buf);
        }
      }
      //如果文件名是.或者..，就跳过
      else if (st.type == T_DIR && strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0)
      {
        find(buf, fileName);
      }
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  int i;

  find(argv[1], argv[2]);
  exit(0);
}
