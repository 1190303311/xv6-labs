#include "kernel/types.h"
#include "user.h"

int main(int argc,char* argv[])
{
    char line[1024];//读入|前面程序的输出
    char *params[100];//参数列表
    int n, k=0,status=-1;
    char *cmd=argv[1];//第二个参数是程序名，第一个参数是xargs
    for(int i=1; i<argc; i++)
        params[k++]=argv[i];
    while((n=read(0,line,1024))>0)//从标准输入读取，sh.c已经把|前面的命令的输出重定位到了xargs的标准输入
    {
        if(fork()==0)
        {
            char *param=(char*)malloc(sizeof(line));
            int index=0;
            for(int i=0; i<n;i++)
            {
                if(line[i]==' '||line[i]=='\n')
                {
                    param[index]=0;
                    index=0;
                    params[k++]=param;
                    param=(char*)malloc(sizeof(line));
                }
                else
                    param[index++]=line[i];
            }
            free(param);
            params[k]=0;
            exec(cmd,params);
        }
        else
        {
            wait(&status);
            exit(0);
        }
    }
    exit(0);
}