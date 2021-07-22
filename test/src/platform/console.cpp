#include <spacepi/Platform.hpp>
#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace spacepi;

static void testPrinting() {
    char filename[L_tmpnam];
    tmpnam(filename);
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
    remove(filename);
    if (strncmp(msg, buf, sizeof(buf))) {
        exit(EXIT_FAILURE);
    } else {
        exit(EXIT_SUCCESS);
    }
}

TEST(spacepi_Platform_printConsole, works) {
    EXPECT_EXIT(testPrinting(), testing::ExitedWithCode(EXIT_SUCCESS), "");
}
