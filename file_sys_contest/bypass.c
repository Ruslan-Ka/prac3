#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <string.h>


void f(const char *dir) {
    char name[PATH_MAX+1];
    DIR *d;
    struct dirent *dd;
    off_t t;
    struct stat s;
    char *dr = "/";
    if (!strcmp(dir, "/")) {
        dr = " ";
    }
    if (!(d = opendir(dir))) {
        return;
    }
    while (dd = readdir(d)) {
        if(!strcmp(dd->d_name, ".") || !strcmp(dd->d_name, "..")) {
            continue;
        }
        snprintf(name, PATH_MAX, "%s%s%s", dir, dr, dd->d_name);
        if (lstat(name, &s) < 0)
            continue;
        if (S_ISDIR(s.st_mode)) {
            t = telldir(d);
            closedir(d);
            f(name);
            if(!(d = opendir(dir))) {
                seekdir(d,t);
            } else {
                printf("%s\n",name);
                closedir(d);
            }
        }
    }
}

int main() {
    f("/home/ruslan/CLionProjects/kr01-5");
    return 0;
}