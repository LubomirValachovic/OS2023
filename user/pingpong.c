#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) 
{
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);

    if(pipe(p1)!=0 || pipe(p2)!=0){
        fprintf(2,"pipe error\n");
        exit(-1);
    }

    int pid= fork();

    if (pid<0) {
        fprintf(2,"fork error\n");
        exit(-1);
    } else
    if (pid==0) {
        // child
        char buf;
        read(p1[0], &buf, 1);
        printf("%d: received ping\n", getpid());
        write(p2[1],"b",1);

    } else{
        // parent
        write(p1[1],"a",1);
         char buf;
        read(p2[0], &buf, 1);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}
