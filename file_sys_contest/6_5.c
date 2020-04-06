#include <sys/stat.h>

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

enum
{
    FOR_OWNER = 6,
    FOR_GROUP = 3
};

int
myaccess(const struct stat *stb, const struct Task *task, int access)
{
    if (task->uid == 0) {
        return 1;
    }
    if (task->uid == stb->st_uid) {
        if ((access & (stb->st_mode >> FOR_OWNER)) == access) {
            return 1;
        } else {
            return 0;
        }
    }
    for(int i = 0; i < task->gid_count; ++i) {
        if (task->gids[i] == stb->st_gid) {
            if ((access & (stb->st_mode >> FOR_GROUP)) == access) {
                return 1;
            } else {
                return 0;
            }
        }
    }
    if ((stb->st_mode & access) == access) {
        return 1;
    } else {
        return 0;
    }
}
