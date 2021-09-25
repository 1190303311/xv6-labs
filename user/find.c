#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  memmove(buf, p, strlen(p)+1);
  return buf;
}

void judge(char *filename, char *name)
{
    if(strcmp(fmtname(filename),name)==0)
        printf("%s\n",filename);
}

void find(char *path, char *filename)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    judge(path,filename);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || de.inum == 1 || strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0)
        continue;
      memmove(p, de.name, strlen(de.name));
      p[strlen(de.name)] = 0;
      find(buf,filename);
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{
    if(argc<3)
    {
        fprintf(2,"usage: find <path> <filename>\n");
        exit(1);
    }
    find(argv[1],argv[2]);
    exit(0);
}
