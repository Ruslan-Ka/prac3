#include <stdio.h>
#include <wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd[2];
    int fd1[2];
    pipe(fd);
    pipe(fd1);
    FILE *f1_in = fdopen(fd[0], "r");
    FILE *f1_out = fdopen(fd[1], "w");
    FILE *f2_in = fdopen(fd1[0], "r");
    FILE *f2_out = fdopen(fd1[1], "w");
    int max, num = 1;
    sscanf(argv[1], "%d", &max);
    if (max <= num) {
        printf("Done\n");
        fflush(stdout);
        exit(0);
    }
    fprintf(f1_out, "%d ", num);
    fflush(f1_out);
    pid_t pid = fork();
    if (pid == -1) {
        fclose(f1_out);
        fclose(f1_in);
        fclose(f2_in);
        fclose(f2_out);
        _exit(1);
    } else if (pid == 0) {
        fclose(f1_out);
        fclose(f2_in);
        while (1) {
            if (fscanf(f1_in, "%d", &num) != EOF) {
                printf("1 %d\n", num);
                fflush(stdout);
                num++;
                if (num < max) {
                    fprintf(f2_out, "%d ", num);
                    fflush(f2_out);
                } else {
                    fclose(f2_in);
                    fclose(f2_out);
                    _exit(0);
                }
            } else {
                fclose(f2_out);
                fclose(f1_in);
                _exit(0);
            }
        }
    }
    pid_t pid1 = fork();
    if (pid1 == -1) {
        fclose(f1_out);
        fclose(f1_in);
        fclose(f2_in);
        fclose(f2_out);
        _exit(1);
    } else if (pid1 == 0) {
        fclose(f2_out);
        fclose(f1_in);
        while (1) {
            if (fscanf(f2_in, "%d", &num) != EOF) {
                printf("2 %d\n", num);
                fflush(stdout);
                num++;
                if (num < max) {
                    fprintf(f1_out, "%d ", num);
                    fflush(f1_out);
                } else {
                    fclose(f1_out);
                    fclose(f2_in);
                }
            } else {
                fclose(f1_out);
                fclose(f2_in);
                _exit(0);
            }
        }
    }
    fclose(f1_out);
    fclose(f2_out);
    fclose(f1_in);
    fclose(f2_in);
    while (wait(0) != -1);
    printf("Done\n");
    fflush(stdout);
    exit(0);
}