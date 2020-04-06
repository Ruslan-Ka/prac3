#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

int main(void) {
    pid_t pid = fork();
    pid_t pid1;
    pid_t pid2;
    int p;
    char *end;
    char buf[8];
    if (pid == -1) {
        exit(1);
    }
    if (pid == 0) {
        read(STDIN_FILENO, &buf, sizeof(buf));
        int num = strtol(buf, &end, 10);
        printf("3 %d\n", num * num);
        exit(0);
    } else {
        pid1 = fork();
        if (pid1 == -1) {
            exit(1);
        }
        if (pid1 == 0) {
            read(STDIN_FILENO, &buf, sizeof(buf));
            int num = strtol(buf, &end, 10);
            printf("2 %d\n", num * num);
            exit(0);
        } else {
            pid2 = fork();
            if (pid2 == -1) {
                exit(1);
            }
            if (pid2 == 0) {
                read(STDIN_FILENO, &buf, sizeof(buf));
                int num = strtol(buf, &end, 10);
                printf("1 %d\n", num * num);
                exit(0);
            }
        }
    }
    while ((p = wait(0)) == -1);
    while ((p = wait(0)) == -1);
    while ((p = wait(0)) == -1);
    return 0;
}