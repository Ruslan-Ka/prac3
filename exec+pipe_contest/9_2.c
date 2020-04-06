#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int mysys(const char *str)
{
    pid_t pid = fork();
    int st;
    if (pid == 0) {
        execlp("/bin/sh", "sh", "-c", str, NULL);
        _exit(127);
    }
    else if (pid == -1) {
        return -1;
    } else {
        waitpid(pid, &st, 0);
        if (WIFSIGNALED(st) != 0) {
            return WTERMSIG(st) + 128;
        }
        if (WIFEXITED(st) != 0) {
            return WEXITSTATUS(st);
        }
    }
    return 0;
}