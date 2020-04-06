#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <signal.h>

struct Message
{
    long type;
    long long num[2];
};

int main(int argc, char *argv[])
{
    key_t key;
    sscanf(argv[1], "%u", &key);
    int st;
    long long n, val1, val2, maxval;
    sscanf(argv[2], "%lld", &n);
    sscanf(argv[3], "%lld", &val1);
    sscanf(argv[4], "%lld", &val2);
    sscanf(argv[5], "%lld", &maxval);
    pid_t *arr = calloc(n, sizeof(*arr));
    int id = msgget(key, IPC_CREAT | 0666);
    struct Message begin;
    begin.type = 1;
    begin.num[0] = val1;
    begin.num[1] = val2;
    for (long long i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            while (1) {
                struct Message cur;
                if (msgrcv(id, &cur, sizeof(cur.num), i + 1, 0) < 0) {
                    _exit(0);
                }
                long long res = cur.num[0] + cur.num[1];
                printf("%lld %lld\n", i, res);
                fflush(stdout);
                if (res > maxval) {
                    msgctl(id, IPC_RMID, NULL);
                    _exit(0);
                }
                begin.type = (res % n) + 1;
                begin.num[0] = cur.num[1];
                begin.num[1] = res;
                msgsnd(id, &begin, sizeof(begin.num), 0);
            }
        }
        else if (pid == -1) {
            msgctl(id, IPC_RMID, NULL);
            for (long long j = 0; j < n; j++) {
                if (arr[j] <= 0) {
                    break;
                } else {
                    kill(arr[j], SIGKILL);
                }
            }
            while (wait(0) != -1);
            exit(1);
        }
        arr[i] = pid;
    }
    msgsnd(id, &begin, sizeof(begin.num), 0);
    while (wait(&st) != -1);
    return 0;
}