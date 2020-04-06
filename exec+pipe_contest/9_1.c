#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int st = 0;
    pid_t pid = fork();
    if (pid == 0) {
        int f1 = open(argv[2], O_RDONLY);
        int f2 = open(argv[3], O_CREAT | O_APPEND | O_WRONLY, 0660);
        int f3 = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0660);
        if (f1 < 0 || f2 < 0 || f3 < 0) {
            exit(42);
        }
        if (dup2(f1, 0) < 0) {
            exit(42);
        }
        if (dup2(f2, 1) < 0) {
            exit(42);
        }
        if (dup2(f3, 2) < 0) {
            exit(42);
        }
        if (close(f1) < 0) {
            exit(42);
        }
        if (close(f2) < 0) {
            exit(42);
        }
        if (close(f3) < 0) {
            exit(42);
        }
        execlp(argv[1], argv[1], NULL);
        exit(42);
    }
    wait(&st);
    printf("%d\n", st);
    return 0;
}
