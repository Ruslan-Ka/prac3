#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int
exec(char *str)
{
    pid_t pid = fork();
    int st;
    if (pid == -1) {
        return 0;
    } else if (pid == 0) {
        execlp(str, str, NULL);
        return 0;
    } else {
        waitpid(pid, &st, 0);
        if (WIFEXITED(st) && !WEXITSTATUS(st)) {
            return 1;
        } else {
            return 0;
        }

    }
}

int main(int argc, char *argv[]) {
    if ((exec(argv[1]) || exec(argv[2])) && exec(argv[3])) {
        return 0;
    } else {
        return 1;
    }
}