#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

int main(void)
{
    int fd[2];
    pipe(fd);
    int a;
    int ain;
    long long sum = 0;
    pid_t pid = fork();
    if (pid == -1) {
        exit(1);
    } else if (pid == 0) {
        if ((pid = fork()) == -1) {
            exit(1);
        } else if (pid == 0) {
            close(fd[1]);
            while (read(fd[0], &ain, sizeof(ain)) == sizeof(ain)) {
                sum += ain;
            }
            printf("%lld\n", sum);
            close(fd[0]);
            exit(0);
        } else {
            close(fd[0]);
            close(fd[1]);
            wait(0);
            exit(0);
        }
    } else {
        while (scanf("%d", &a) != EOF) {
            write(fd[1], &a, sizeof(a));
        }
        close(fd[0]);
        close(fd[1]);
        while (wait(0) != -1);
        exit(0);
    }
}