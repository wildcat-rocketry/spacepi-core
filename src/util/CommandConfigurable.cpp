#include <exception>
#include <memory>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/util/Exception.hpp>

using namespace std;
using namespace boost::program_options;
using namespace spacepi::util;

CommandConfigurable::CommandConfigurable(const string &caption, vector<string> &args) : caption(caption), args(args), constructed(false) {
}

CommandConfigurable::~CommandConfigurable() {
    if (!constructed) {
        try {
            throw EXCEPTION(CommandConfigurableException("CommandConfigurable::construct() must be called inside the child class constructor"));
        } catch (const std::exception &) {
            std::terminate();
        }
    }
}

vector<string> CommandConfigurable::parse(int argc, const char **argv) {
    vector<string> args;
    for (int i = 0; i < argc; ++i) {
        args.push_back(string(argv[i]));
    }
    return args;
}

void CommandConfigurable::construct() {
    if (constructed) {
        throw EXCEPTION(CommandConfigurableException("CommandConfigurable::construct() must be called only once inside the child class constructor"));
    }
    constructed = true;
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
