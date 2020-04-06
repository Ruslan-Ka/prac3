#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    int fd[2][2];
    double dx, x0;
    int n;
    sscanf(argv[1], "%lf", &x0);
    sscanf(argv[2], "%d", &n);
    sscanf(argv[3], "%lf", &dx);
    pipe(fd[0]);
    pipe(fd[1]);
    if (!fork()) {
        if (!fork()) {
            close(fd[0][0]);
            close(fd[1][1]);
            for (int i = 0; i <= n; i++) {
                double tmp =  sin(x0 + i * dx);
                if(write(fd[0][1], &tmp, sizeof(tmp)) != sizeof(tmp)) {
                    _exit(0);
                }
            }
            double res = 0;
            for (int i = 0; i <= n; i++) {
                double tmp = 0;
                if(read(fd[1][0], &tmp, sizeof(tmp)) != sizeof(tmp)) {
                    _exit(0);
                }
                res += tmp * tmp;
            }
            printf("1 %.10g\n", res);
            close(fd[0][1]);
            close(fd[1][0]);
            return 0;
        }
        wait(NULL);
        close(fd[0][0]);
        close(fd[0][1]);
        close(fd[1][0]);
        close(fd[1][1]);
        return 0;
    }
    if (!fork()) {
        if (!fork()) {
            close(fd[1][0]);
            close(fd[0][1]);
            double res = 0;
            for (int i = 0; i <= n; i++) {
                double tmp = 0;
                if(read(fd[0][0], &tmp, sizeof(tmp)) != sizeof(tmp)) {
                    _exit(0);
                }
                res += fabs(tmp);
            }
            for (int i = 0; i <= n; i++) {
                double tmp = cos(x0 + i * dx);
                if (write(fd[1][1], &tmp, sizeof(tmp)) != sizeof(tmp)) {
                    _exit(0);
                }
            }
            printf("2 %.10g\n", res);
            close(fd[1][1]);
            close(fd[0][0]);
            return 0;
        }
        wait(NULL);
        close(fd[0][0]);
        close(fd[0][1]);
        close(fd[1][0]);
        close(fd[1][1]);
        return 0;
    }
    close(fd[0][0]);
    close(fd[0][1]);
    close(fd[1][0]);
    close(fd[1][1]);
    while(wait(NULL)!= -1);
    printf("0 0\n");
    return 0;
}