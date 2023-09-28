#include "kernel/types.h"
#include "user/user.h"

void pipe_process(int fd)
{
    int num;
    read(fd, &num, sizeof(int));
     printf("prime %d\n", num);
    int next_pipe[2];
    pipe(next_pipe);

    int next_num = -1;
    while (read(fd, &next_num, sizeof(int)) > 0)
    {
        if (next_num % num != 0)
        {
            write(next_pipe[1], &next_num, sizeof(int));
        }
    }

    if (next_num == -1) {
        // Empty pipe
        close(next_pipe[0]);
        close(next_pipe[1]);
        close(fd);
        return;
    }

    int pid = fork();
    if (pid == 0)
    {
        // Child process
        close(next_pipe[1]);
        close(fd);
        pipe_process(next_pipe[0]);
        close(next_pipe[0]);
    }
    else
    {
        // Parent process
        close(next_pipe[1]);
        close(next_pipe[0]);
        close(fd);
        wait(0);
    }
}

int main(int argc, char* argv[])
{
    int p[2];
    // TODO: check pipe return value
    pipe(p);

    for (int i = 2; i <= 35; i++)
        write(p[1], &i, sizeof(int));

    close(p[1]);
    pipe_process(p[0]);
    close(p[0]);

    exit(0);
}