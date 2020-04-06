#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <wait.h>

int fd[2];
FILE *f1_in;
int max;
FILE *f1_out;

void f(int sig) {
    int pid, a;
    fscanf(f1_in, "%d", &pid);
    if (fscanf(f1_in, "%d", &a) != EOF) {
        if (a % 2 == 1) {
            printf("1 %d\n", a);
            fflush(stdout);
        } else {
            printf("2 %d\n", a);
            fflush(stdout);;
        }
        a++;
        if (a >= max) {
            kill(pid, SIGKILL);
            exit(0);
        }
        fprintf(f1_out, "%d %d ", getpid(), a);
        fflush(f1_out);
        kill(pid, SIGUSR1);
    } else {
        kill(pid, SIGKILL);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    struct sigaction sig;
    sig.sa_handler = f;
    sig.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sig, NULL);
    pipe(fd);
    f1_in = fdopen(fd[0], "r");
    f1_out = fdopen(fd[1], "w");
    int cur = 1;
    sscanf(argv[1], "%d", &max);
    if (fork() == 0) {
        int check;
        fscanf(f1_in, "%d", &check);
        if (check == 1) {
            exit(0);
        }
        kill(getpid(), SIGUSR1);
        while (1);
    }
    int check = 0;
    if (fork() == 0) {
        if (cur < max) {
            fprintf(f1_out, "%d %d %d ", check, getpid(), cur);
            fflush(f1_out);
        } else {
            check = 1;
            fprintf(f1_out, "%d %d ", check, getpid());
            fflush(f1_out);
            exit(0);
        }
        while (1);
    }
    fclose(f1_out);
    fclose(f1_in);
    while (wait(0) != -1);
    printf("Done\n");
    fflush(stdout);
    return 0;
}