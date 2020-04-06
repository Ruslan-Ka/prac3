#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[])
{
    setbuf(stdin, NULL);
    int n;
    sscanf(argv[1], "%d", &n);
    int id = semget(IPC_PRIVATE,n, IPC_CREAT | 0777);
    for (int i = 0; i < n; i++) {
        if (fork() == 0) {
            while (1) {
                int num;
                struct sembuf op1 = {i, -1, 0};
                if (semop(id, &op1, 1) < 0) {
                    _exit(0);
                }
                if (scanf("%d", &num) != EOF) {
                    printf("%d %d\n", i, num);
                    fflush(stdout);
                    int next = num % n;
                    if (next < 0) {
                        next = next + n;
                    }
                    struct sembuf op2 = {next, 1, 0};
                    semop(id, &op2, 1);
                } else {
                    semctl(id, IPC_RMID, 0);
                    _exit(0);
                }
            }
        }
    }
    for (int i = 0; i < n; i++) {
        semctl(id, i, SETVAL, 0);
    }
    struct sembuf start = {0,1,0};
    semop(id, &start, 1);
    while (wait(0) != -1);
    exit(0);
}