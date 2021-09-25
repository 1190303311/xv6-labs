#include "kernel/types.h"
#include "user.h"

int main(int argc, char *argv[])
{
    int p1[2],p2[2];
    pipe(p1);//p1 child reads from parent
    pipe(p2);//p2 child write to parent
    if(argc>1)
    {
        fprintf(2,"usage:pingpang\n");
        exit(1);
    }
    if(fork()==0)
    {
        char c[3];
        int n;
        n = read(p1[0],c,1);
        if(n<=0)
        {
            fprintf(2,"read error from child\n");
            exit(1);
        }
        if(c[0]=='a')
        {
            fprintf(1,"%d: received ping\n",getpid());
            n = write(p2[1],"b",1);
            if(n!=1)
            {
                fprintf(2,"write error from child\n");
                exit(1);
            }
        }
    }
    else
    {
        char c[3];
        int n;
        n = write(p1[1],"a",1);
        if(n!=1)
        {
            fprintf(2,"write error from parent\n");
            exit(1);
        }
        n = read(p2[0],c,1);
        if(n<=0)
        {
            fprintf(2,"read error from parent\n");
            exit(1);
        }
        if(c[0]=='b')
        {
            fprintf(1,"%d: received pong\n",getpid());
        }
    }
    close(p1[0]);
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);
    exit(0);
}