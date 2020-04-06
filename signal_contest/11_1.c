#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

enum
{
    STOP = 5
};

int sig_count = 0;

void
f_wait(int sig)
{
    if (sig_count < STOP) {
        printf("%d\n", sig_count++);
        fflush(stdout);
    } else {
        _exit(0);
    }
}

int main(void)
{
    struct sigaction action;
    sigset_t sig;
    sigemptyset(&sig);
    sigaddset(&sig, SIGHUP);
    action.sa_flags = SA_RESTART;
    action.sa_handler = f_wait;
    sigaction(SIGHUP, &action, NULL);
    sigprocmask(SIG_BLOCK, NULL, &sig);
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        sigsuspend(&sig);
    }
}