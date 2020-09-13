#include <memory>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <spacepi/util/CommandConfigurable.hpp>

using namespace std;
using namespace boost::program_options;
using namespace spacepi::util;

CommandConfigurable::CommandConfigurable(const string &caption, vector<string> &args) {
    init(caption, args);
}

CommandConfigurable::~CommandConfigurable() {
}

vector<string> CommandConfigurable::parse(int argc, const char **argv) {
    vector<string> args;
    for (int i = 0; i < argc; ++i) {
        args.push_back(string(argv[i]));
    }
    return args;
}

void CommandConfigurable::init(const string &caption, vector<string> &args) {
    options_description desc(caption);
    options(desc);
    string arg0 = args.front();
    parsed_options opts = command_line_parser(args)
        .options(desc)
        .allow_unregistered()
        .run();
    args = collect_unrecognized(opts.options, include_positional);
    args.insert(args.begin(), arg0);
    configure(opts);
}
