#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

int main(void) {
    int p;
    pid_t pid = fork();
    if (pid == -1) {
        exit(1);
    }
    if (pid == 0) {
        pid_t pid1 = fork();
        if (pid1 == -1) {
            exit(1);
        }
        if(pid1 == 0) {
            printf("3 ");
            exit(0);
        }
        if (pid1 != 0) {
            while ((p = wait(0)) == -1);
            printf("2 ");
            exit(0);
        }
    } else {
        while ((p = wait(0)) == -1);
        printf("1\n");
    }
    return 0;
}