#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
enum
{
    END = 3
};

volatile int kol = 0;
volatile int res = 0;

void
f2(int sig)
{
    exit(0);
}

void
f1(int sig)
{
    if (kol < END) {
        printf("%d\n", res);
        fflush(stdout);
        kol++;
    }
    else {
        exit(0);
    }
}

int main(void)
{
    int low, high;
    scanf("%d%d", &low, &high);
    printf("%d\n", getpid());
    fflush(stdout);
    struct sigaction s1;
    struct sigaction s2;
    int cur = 1;
    s1.sa_handler = f1;
    s2.sa_handler = f2;
    s1.sa_flags = SA_RESTART;
    s2.sa_flags = SA_RESTART;
    sigaction(SIGINT, &s1, NULL);
    sigaction(SIGTERM, &s2, NULL);
    for (int i = low; i < high; i++) {
        if (i < 2) {
            continue;
        }
        cur = 1;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                cur = 0;
                break;
            }
        }
        if (cur == 1) {
            res = i;
        }
    }
    printf("-1\n");
    fflush(stdout);
    exit(0);
}