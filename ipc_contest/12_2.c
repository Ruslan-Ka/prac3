#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int count, nproc, iter_count, seed;
    key_t key;
    sscanf(argv[1], "%d", &count);
    sscanf(argv[2], "%d", &key);
    sscanf(argv[3], "%d", &nproc);
    sscanf(argv[4], "%d", &iter_count);
    sscanf(argv[5], "%d", &seed);
    int memid = shmget(key, sizeof(int) * count, IPC_CREAT | 0666);
    int *arr = shmat(memid, 0, 0);
    int semid = semget(key, count, IPC_CREAT | 0666);
    for (int i = 0; i < count; i++) {
        scanf("%d", &arr[i]);
    }
    for (int i = 0; i < count; i++) {
        semctl(semid, i, SETVAL, 1);
    }
    for (int i = 0; i < nproc; i++) {
        if (fork() == 0) {
            srand(seed);
            for(int j = 0; j < iter_count; j++) {
                int ind1 = rand() % count;
                int ind2 = rand() % count;
                int value = rand() % 10;
                struct sembuf op[2];
                op[0].sem_num = ind1;
                op[1].sem_num = ind2;
                op[0].sem_op = -1;
                op[1].sem_op = -1;
                op[0].sem_flg = 0;
                op[1].sem_flg = 0;
                if (ind1 == ind2) {
                    if (semop(semid, &op[0], 1) < 0) {
                        _exit(0);
                    }
                } else if (semop(semid, op, 2) < 0) {
                    _exit(0);
                }
                operation(arr,ind1, ind2, value);
                op[1].sem_op = 1;
                op[0].sem_op = 1;
                if (ind1 == ind2) {
                    if (semop(semid, &op[0], 1) < 0) {
                        _exit(0);
                    }
                } else if (semop(semid, op, 2) < 0) {
                    _exit(0);
                }
            }
            _exit(0);
        }
    }
    while (wait(0) != -1);
    semctl(semid, IPC_RMID, 0);
    for (int i = 0; i < count; i++) {
        printf("%d\n", arr[i]);
        fflush(stdout);
    }
    shmctl(memid, IPC_RMID, 0);
    return 0;
}