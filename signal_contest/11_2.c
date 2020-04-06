#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

volatile int check = 0;

void f1(int sig)
{
    check = 0;
}

void f2(int sig)
{
    check = 1;
}

int main(void)
{
    struct sigaction act1;
    struct sigaction act2;
    act1.sa_handler = f1;
    act2.sa_handler = f2;
    act1.sa_flags = SA_RESTART;
    act2.sa_flags = SA_RESTART;
    sigaction(SIGINT, &act1, NULL);
    sigaction(SIGQUIT, &act2, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    int cur = 0;
    int a;
    while (scanf("%d", &a) == 1) {
        if (check == 0) {
            __builtin_add_overflow(cur, a, &cur);
            printf("%d\n", cur);
            fflush(stdout);
        } else {
            __builtin_mul_overflow(cur, a, &cur);
            printf("%d\n", cur);
            fflush(stdout);
        }
    }
    return 0;
}