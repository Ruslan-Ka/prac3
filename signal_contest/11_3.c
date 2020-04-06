#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <wait.h>

int main(int argc, char *argv[]) {
    int n, res = 0, tmp = argc;
    int st;
    sscanf(argv[1], "%d", &n);
    for (int i = 2; i < argc; i++) {
        if (i - 2  == n) {
            tmp = i;
            break;
        }
        if (fork() == 0) {
            char s[PATH_MAX];
            FILE *f = fopen(argv[i], "r");
            fscanf(f,"%s", s);
            fclose(f);
            execlp(s,s,NULL);
            _exit(1);
        }
    }
    while (wait(&st) != -1) {
        if (WIFEXITED(st) && !WEXITSTATUS(st)) {
            res++;
        }
    }
    for (int i = tmp; i < argc; i++) {
        if (fork() == 0) {
            char s[PATH_MAX];
            FILE *f = fopen(argv[i], "r");
            fscanf(f,"%s", s);
            fclose(f);
            execlp(s,s,NULL);
            _exit(1);
        }
        wait(&st);
        if (WIFEXITED(st) && !WEXITSTATUS(st)) {
            res++;
        }
    }
    printf("%d\n", res);
    fflush(stdout);
    return 0;
}