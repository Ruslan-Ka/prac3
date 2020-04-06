#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int main(void)
{
    int n;
    int p;
    scanf("%d", &n);
    for(int i = 1; i <= n; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            exit(1);
        }
        if (pid == 0) {
            if (i != n) {
                printf("%d ", i);
            } else {
                printf("%d\n", i);
                exit(0);
            }
        } else {
            while ((p = wait(0)) == -1);
            _exit(0);
        }
    }
    return 0;
}