#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    DIR *d = opendir(argv[1]);
    unsigned long long res = 0;
    struct dirent *enter;
    char slash = '/';
    if (d != NULL) {
        while ((enter = readdir(d)) != NULL) {
            if (strcmp(enter->d_name, "..") != 0 && strcmp(enter->d_name, ".") != 0) {
                char buf[PATH_MAX + 1];
                if (strlen(argv[1]) + strlen(enter->d_name) + 1 <= PATH_MAX) {
                    snprintf(buf, PATH_MAX, "%s%c%s", argv[1], slash, enter->d_name);
                    struct stat cur;
                    int a = stat(buf, &cur);
                    if (a != -1 && S_ISREG(cur.st_mode) == 1 && isupper(enter->d_name[0]) &&
                        cur.st_uid == getuid()) {
                        res += cur.st_size;
                    }
                }
            }
        }
    }
    printf("%llu\n", res);
    return 0;
}