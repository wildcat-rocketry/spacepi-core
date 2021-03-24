#define _GNU_SOURCE
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <sched.h>
#include <unistd.h>

int main(int argc, char **argv) {
    struct statfs stat;
    if (statfs("/", &stat) < 0) {
        fprintf(stderr, "statfs: %s\n", strerror(errno));
    } else if (stat.f_flags & MS_RDONLY) {
        uid_t uid = getuid();
        if (setuid(0) < 0) {
            fprintf(stderr, "setuid(0): %s\n", strerror(errno));
        }

        if (unshare(CLONE_NEWNS) < 0) {
            fprintf(stderr, "unshare: %s\n", strerror(errno));
        } else if (mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL) < 0) {
            fprintf(stderr, "mount(MS_REC|MS_PRIVATE): %s\n", strerror(errno));
        } else if (mount(NULL, "/", NULL, MS_REMOUNT | (stat.f_flags & ~MS_RDONLY), NULL) < 0) {
            fprintf(stderr, "mount(MS_REMOUNT): %s\n", strerror(errno));
        }

        if (setuid(uid) < 0) {
            fprintf(stderr, "setuid(%d): %s\n", uid, strerror(errno));
        }
        if (seteuid(uid) < 0) {
            fprintf(stderr, "seteuid(%d): %s\n", uid, strerror(errno));
        }
    }

    char **childArgv = malloc(sizeof(char *) * (argc + 1));
    *childArgv = "/bin/bash";
    for (int i = 1; i < argc; ++i) {
        childArgv[i] = argv[i];
    }
    childArgv[argc] = NULL;

    execv("/bin/bash", childArgv);
    free(childArgv);
    fprintf(stderr, "execl: %s\n", strerror(errno));
    return EXIT_FAILURE;
}
