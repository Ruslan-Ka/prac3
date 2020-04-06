#include <string.h>

const char temp[] = "rwxrwxrwx";

int
parse_rwx_permissions(const char *str)
{
    if (!str) {
        return -1;
    }
    int size = strlen(str);
    if (size != sizeof(temp) - 1) {
        return -1;
    }
    int res = 0;
    for (int i = 0; i  < size; i++) {
        if(str[i] != '-' && str[i] != temp[i]) {
            return -1;
        }
        if(str[i] == temp[i]) {
            res |= 1;
        }
        res <<= 1;
    }
    return res >> 1;
}