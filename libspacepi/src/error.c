#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <MQTTAsync.h>
#include <spacepi.h>

//#define RELATIVIZE_DEBUG

static const char *const spacepi_errors[] = {
    "Library already initialized",
    "Cascaded error",
    "Already connected",
    "Library not initialized",
    "Unknown error"
};
static int n_spacepi_errors = sizeof(spacepi_errors) / sizeof(spacepi_errors[0]);

static const char absolute_path[] = __FILE__;
static const char relative_path[] = "support/libspacepi/src/error.c";

#ifdef RELATIVIZE_DEBUG
static const char *relativize(const char *file);
#else
#define relativize(x) x
#endif

const char *spacepi_strerror(spacepi_error_t spacepi_errno) {
    const char *e;
    switch (spacepi_errno.data.type) {
        case system_err:
            return strerror(spacepi_errno.data.code);
        case spacepi:
            if (spacepi_errno.data.code < n_spacepi_errors) {
                return spacepi_errors[spacepi_errno.data.code];
            }
            break;
        case mqtt:
            e = MQTTAsync_strerror(~spacepi_errno.data.code);
            if (e) {
                return e;
            }
            break;
        default:
            break;
    }
    return spacepi_errors[UNKNOWN_ERROR];
}

void spacepi_perror(const char *func, const char *file, int line) {
    fflush(stdout);
    spacepi_error_t error;
    error.error_code = errno;
    if (error.data.type == spacepi && error.data.code == SPACEPI_ERROR_CASCADE) {
        fprintf(stderr, "  at %s:%d\n", relativize(file), line);
    } else {
        fprintf(stderr, "%s: %s (0x%0"
#if INT_MAX == 127
            "2"
#elif INT_MAX == 32767
            "4"
#elif INT_MAX == 2147483647
            "8"
#elif INT_MAX == 9223372036854775807
            "16"
#else
#error Invalid int size.
#endif
            "X)\n  at %s:%d\n", func, spacepi_strerror(error), error.error_code, relativize(file), line);
    }
}

void spacepi_trace(const char *file, int line) {
    fflush(stdout);
    fprintf(stderr, "(Trace at %s:%d)\n", relativize(file), line);
}

#ifdef RELATIVIZE_DEBUG
static const char *relativize(const char *file) {
    int n = sizeof(absolute_path) - sizeof(relative_path);
    if (strcmp(absolute_path + n, relative_path) == 0 && strncmp(absolute_path, file, n) == 0) {
        return file + n;
    } else {
        return file;
    }
}
#endif
