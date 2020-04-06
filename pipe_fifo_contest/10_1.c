#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <wait.h>
enum
{
    BASE_YEAR = 1900
};
int main(void) {
    int fd[2];
    pipe(fd);
    struct tm *cur;
    time_t curtime;
    pid_t pid = fork();
    if (pid == -1) {
        exit(1);
    } else if (pid == 0) {
        pid = fork();
        if (pid == -1) {
            exit(1);
        } else if (pid == 0) {
            pid = fork();
            if (pid == -1) {
                exit(1);
            } else if (pid == 0) {
                curtime = time(NULL);
                write(fd[1], &curtime, sizeof(curtime));
                write(fd[1], &curtime, sizeof(curtime));
                write(fd[1], &curtime, sizeof(curtime));
                close(fd[0]);
                close(fd[1]);
                exit(0);
            } else {
                wait(0);
                read(fd[0], &curtime, sizeof(curtime));
                cur = localtime(&curtime);
                printf("D:%d\n", cur->tm_mday);
                close(fd[0]);
                close(fd[1]);
                exit(0);
            }
        } else {
            while (wait(0) != -1);
            read(fd[0], &curtime, sizeof(curtime));
            cur = localtime(&curtime);
            printf("M:%d\n", cur->tm_mon + 1);
            close(fd[0]);
            close(fd[1]);
            exit(0);
        }
    } else {
        while (wait(0) != -1);
        read(fd[0], &curtime, sizeof(curtime));
        cur = localtime(&curtime);
        printf("Y:%d\n", cur->tm_year + BASE_YEAR);
        close(fd[0]);
        close(fd[1]);
        exit(0);
    }
}