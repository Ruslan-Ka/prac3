#include <stdio.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    unsigned long long result = 0;
    for (int i = 1; i < argc; ++i) {
        struct stat f;
        if (lstat(argv[i], &f) >= 0) {
            if (S_ISREG(f.st_mode) && f.st_nlink == 1) {
                result += f.st_size;
            }
        }
    }
    printf("%llu\n", result);
    return 0;
}