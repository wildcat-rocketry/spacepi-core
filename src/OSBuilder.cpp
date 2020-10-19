#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <spacepi/target/rpi/OSBuilder.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

using namespace std;
using namespace boost::program_options;
using namespace spacepi::target::rpi;
using namespace spacepi::util;

OSBuilder::OSBuilder(vector<string> &args) : CommandConfigurable("OS Builder", args) {
    construct();
}

void OSBuilder::run() {
    cout << "This is doing something." << endl;
    cerr << "This could be an error message." << endl;
    ofstream file(outFile);
    file << "Config File: " << configFile << endl
         << "Output File: " << outFile << endl
         << "   Data Dir: " << dataDir << endl;
    cout << "File written to " << outFile << endl;
}

void OSBuilder::options(options_description &desc) const {
    desc.add_options()
        ("config-file", value<string>(), "XML file path to configure the OS from")
        ("out", value<string>(), "Output image file path")
        ("data-dir", value<string>(), "The source directory of the OS target to get data files from");
}

void OSBuilder::configure(const parsed_options &opts) {
    variables_map vm;
    store(opts, vm);
    notify(vm);
    configFile = vm["config-file"].as<string>();
    outFile = vm["out"].as<string>();
    dataDir = vm["data-dir"].as<string>();
}
