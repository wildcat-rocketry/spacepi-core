#include <stdlib.h>
#include "config.h"
#include "logger.h"
#include "program.h"
#include "programlist.h"
#include "resolver.h"

// I'd recommend starting by writing main(), and then work your way through the
// other files based on the order you call them in main().  I'd save logging for
// last since it's not as important as getting the programs to run.  Just make
// sure that if you haven't implemented logging yet you don't try to send data
// to the logging pipes from the child processes or it'll deadlock.

int main(int argc, const char **argv) {
    // TODO
    // Read command-line options/config
    // Read program list file
    // Set callbacks
    // Create loggers
    // Start all the programs
    // Don't exit until SIGINT
    // Make sure to clean up all your resources
    return EXIT_SUCCESS;
}
