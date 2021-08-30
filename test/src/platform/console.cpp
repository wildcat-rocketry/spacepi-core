#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtest/gtest.h>
#include <spacepi/BuildConfig.hpp>
#include <spacepi/Platform.hpp>

using namespace spacepi;

static void testPrinting() {
    char filename[BuildConfig::CMAKE_CURRENT_BINARY_DIR_len() + sizeof("/platform_console_stdout.txt")];
    snprintf(filename, sizeof(filename), "%s/platform_console_stdout.txt", BuildConfig::CMAKE_CURRENT_BINARY_DIR());
    freopen(filename, "w+", stdout);
    const char msg[] = "Hello, world!";
    Platform::printConsole(msg, (int) strlen(msg));
    fflush(stdout);
    fseek(stdout, 0, SEEK_SET);
    char buf[sizeof(msg) + 1];
    int r = (int) fread(buf, 1, sizeof(buf), stdout);
    if (r < sizeof(buf)) {
        buf[r] = '\0';
    }
    fclose(stdout);
    if (strncmp(msg, buf, sizeof(buf))) {
        exit(EXIT_FAILURE);
    } else {
        exit(EXIT_SUCCESS);
    }
}

TEST(Platform, printConsole) {
    EXPECT_EXIT(testPrinting(), testing::ExitedWithCode(EXIT_SUCCESS), "");
}
