#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>


int main(int argc, char *argv[])
{
    int fd[2];
    int fd1[2];
    pid_t *arr_pid = calloc(argc-1, sizeof(*arr_pid));
    pid_t pid;
    for (int i = 1; i < argc; i++) {
        if (i != argc - 1) {
            if (i % 2 == 1) {
                if (pipe(fd) < 0) {
                    for (int j = 0; j < argc - 1; j++) {
                        if (arr_pid[j] <= 0) {
                            break;
                        } else {
                            kill(arr_pid[j], SIGKILL);
                        }
                    }
                    free(arr_pid);
                    close(fd[1]);
                    close(fd[0]);
                    close(fd1[0]);
                    close(fd1[1]);
                    _exit(1);
                }
            } else {
                if (pipe(fd1) < 0) {
                    for (int j = 0; j < argc - 1; j++) {
                        if (arr_pid[j] <= 0) {
                            break;
                        } else {
                            kill(arr_pid[j], SIGKILL);
                        }
                    }
                    free(arr_pid);
                    close(fd[1]);
                    close(fd[0]);
                    close(fd1[0]);
                    close(fd1[1]);
                    _exit(1);
                }
            }
        }
        pid = fork();
        if (pid == -1) {
            for (int j = 0; j < argc - 1; j++) {
                if (arr_pid[j] <= 0) {
                    break;
                } else {
                    kill(arr_pid[j], SIGKILL);
                }
            }
            free(arr_pid);
            close(fd[1]);
            close(fd[0]);
            close(fd1[0]);
            close(fd1[1]);
            _exit(1);
        } else if (pid == 0) {
            if (i == 1) {
                dup2(fd[1], 1);
                close(fd[1]);
                close(fd[0]);
                execlp(argv[i], argv[i], NULL);
                _exit(1);
            } else if (i != argc -1) {
                if (i % 2 == 1) {
                    dup2(fd[1], 1);
                    dup2(fd1[0],0);
                    close(fd[1]);
                    close(fd[0]);
                    close(fd1[0]);
                    close(fd1[1]);
                } else {
                    dup2(fd1[1], 1);
                    dup2(fd[0],0);
                    close(fd[1]);
                    close(fd[0]);
                    close(fd1[0]);
                    close(fd1[1]);
                }
                execlp(argv[i], argv[i], NULL);
                _exit(1);
            } else {
                if (i % 2 == 1) {
                    dup2(fd1[0],0);
                    close(fd1[0]);
                    close(fd1[1]);
                } else {
                    dup2(fd[0],0);
                    close(fd[1]);
                    close(fd[0]);
                }
                execlp(argv[i], argv[i], NULL);
                _exit(1);
            }
        } else {
            arr_pid[i - 1] = pid;
            if (i >= 2) {
                if (i % 2 == 1) {
                    close(fd1[1]);
                    close(fd1[0]);
                } else {
                    close(fd[1]);
                    close(fd[0]);
                }
            }
        }
    }
    while (wait(0) != -1);
    free(arr_pid);
    exit(0);
}

