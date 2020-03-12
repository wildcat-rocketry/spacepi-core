#ifndef CONFIG_H
#define CONFIG_H

#include <linux/limits.h>

typedef enum {
    pm_spacepi_bin, // Run out of {baseFolder}/{programName}
    pm_spacepi_build, // Run out of {baseFolder}/{pathToProgram}/{programName}
    pm_show_help // Just print help and exit
} program_mode_t;

typedef struct {
    program_mode_t mode;
    char baseDir[PATH_MAX];
    char programListFile[PATH_MAX];
} config_t;

int parse_config(config_t *config, int argc, const char **argv);
int print_help(const char *argv0);

#endif
