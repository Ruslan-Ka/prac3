#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd[2];
    int st;
    pipe(fd);
    pid_t pid = fork();
    if (pid == -1) {
        exit(1);
    } else if (pid == 0) {
        close(fd[0]);
        if (fork() == 0) {
            int f = open(argv[4], O_RDONLY);
            dup2(f, 0);
            dup2(fd[1], 1);
            close(f);
            close(fd[1]);
            execlp(argv[1], argv[1], NULL);
            _exit(1);
        } else {
            wait(&st);
            if (WIFEXITED(st) && !WEXITSTATUS(st)) {
                if (fork() == 0) {
                    dup2(fd[1], 1);
                    close(fd[1]);
                    execlp(argv[2], argv[2], NULL);
                    _exit(1);
                } else {
                    wait(0);
                }
            }
        }
        close(fd[1]);
        exit(0);
    }
    pid_t pid1 = fork();
    if (pid1 == -1) {
        exit(1);
    } else if (pid1 == 0) {
        int f_out = open(argv[5], O_CREAT | O_APPEND | O_WRONLY, 0777);
        dup2(fd[0], 0);
        dup2(f_out, 1);
        close(fd[0]);
        close(fd[1]);
        close(f_out);
        execlp(argv[3], argv[3],NULL);
        _exit(1);
    } else {
        close(fd[0]);
        close(fd[1]);
        while (wait(0) != -1);
        exit(0);
    }
}
