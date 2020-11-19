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

static const string invalidCaption;
static vector<string> invalidArgs;

CommandConfigurable::CommandConfigurable(const string &caption, vector<string> &args) noexcept : caption(caption), args(args), constructed(false) {
}

CommandConfigurable::CommandConfigurable(const CommandConfigurable &copy) noexcept : caption(invalidCaption), args(invalidArgs), constructed(true) {
    if (!copy.constructed) {
        try {
            throw EXCEPTION(CommandConfigurableException("CommandConfigurable::construct() must be called inside the child class constructor before copy constructing"));
        } catch (const exception &) {
            terminate();
        }
    }
}

CommandConfigurable::CommandConfigurable(const CommandConfigurable &&move) noexcept : caption(invalidCaption), args(invalidArgs), constructed(true) {
    if (!move.constructed) {
        try {
            throw EXCEPTION(CommandConfigurableException("CommandConfigurable::construct() must be called inside the child class constructor before move constructing"));
        } catch (const exception &) {
            terminate();
        }
    }
}

CommandConfigurable::~CommandConfigurable() {
    if (!constructed) {
        try {
            throw EXCEPTION(CommandConfigurableException("CommandConfigurable::construct() must be called inside the child class constructor"));
        } catch (const exception &) {
            terminate();
        }
    }
}

CommandConfigurable &CommandConfigurable::operator =(const CommandConfigurable &copy) noexcept {
    if (!copy.constructed) {
        try {
            throw EXCEPTION(CommandConfigurableException("CommandConfigurable::construct() must be called inside the child class constructor before copy assigning"));
        } catch (const exception &) {
            terminate();
        }
    }
    constructed = true;
    return *this;
}

CommandConfigurable &CommandConfigurable::operator =(const CommandConfigurable &&move) noexcept {
    if (!move.constructed) {
        try {
            throw EXCEPTION(CommandConfigurableException("CommandConfigurable::construct() must be called inside the child class constructor before move assigning"));
        } catch (const exception &) {
            terminate();
        }
    }
    constructed = true;
    return *this;
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
        try {
            throw EXCEPTION(CommandConfigurableException("CommandConfigurable::construct() must be called only once inside the child class constructor"));
        } catch (const exception &) {
            terminate();
        }
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
