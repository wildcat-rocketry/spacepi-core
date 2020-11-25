#include <fstream>
#include <iostream>
#include <SpacePi.hpp>
#include <spacepi/target/OSBuilder.hpp>

using namespace std;
using namespace spacepi::util;
using namespace spacepi::target;

OSBuilder::OSBuilder(Command &cmd) : CommandConfigurable("OS Builder", cmd) {
    fromCommand(configFile, "config-file", "XML file path to configure the OS from");
    fromCommand(outFile, "out", "Output image file path");
    fromCommand(dataDir, "data-dir", "The source directory of the OS target to get data files from");
}

void OSBuilder::runCommand() {
    cout << "This is doing something." << endl;
    cerr << "This could be an error message." << endl;
    ofstream file(outFile);
    file << "Config File: " << configFile << endl
         << "Output File: " << outFile << endl
         << "   Data Dir: " << dataDir << endl;
    cout << "File written to " << outFile << endl;
}
