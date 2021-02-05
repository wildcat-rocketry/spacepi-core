#include <exception>
#include <iomanip>
#include <string>
#include <vector>
#include <utility>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/LogStream.hpp>
#include <spacepi/util/Command.hpp>
#include <spacepi/util/CommandConfigurable.hpp>
#include <spacepi/util/CommandInternals.hpp>
#include <spacepi/util/Exception.hpp>

using namespace std;
using namespace spacepi::log;
using namespace spacepi::util;
using namespace spacepi::util::detail;

CommandHelpWrapper::CommandHelpWrapper(Command &cmd) noexcept : CommandConfigurable("Global Options", cmd) {
    fromCommand(showHelp, false, "help", "Prints this help message then exits");
}

void CommandHelpWrapper::runCommand() {
}

Command::Command(int argc, const char **argv) noexcept : helpWrapper(*this), logFilter(*this) {
    args.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
}

bool Command::run() noexcept {
    bool success = true;
    for (vector<string>::const_iterator ait = args.begin() + 1; ait != args.end(); ) {
        bool found = false;
        for (vector<CommandGroup>::iterator git = groups.begin(); git != groups.end(); ++git) {
            for (vector<shared_ptr<GenericCommandParser>>::iterator oit = git->options.begin(); oit != git->options.end(); ++oit) {
                pair<vector<string>::const_iterator, string> res = (*oit)->parse(args, ait);
                if (res.first != ait) {
                    if (!res.second.empty()) {
                        log(LogLevel::Error) << res.second;
                        success = false;
                    }
                    ait = res.first;
                    git = groups.end() - 1;
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            log(LogLevel::Error) << "Unknown argument '" << *ait << "'";
            success = false;
            ++ait;
        }
    }
    if (success) {
        for (vector<CommandGroup>::const_iterator git = groups.begin(); git != groups.end(); ++git) {
            for (vector<shared_ptr<GenericCommandParser>>::const_iterator oit = git->options.begin(); oit != git->options.end(); ++oit) {
                if (!**oit) {
                    success = false;
                    log(LogLevel::Error) << "Required argument '" << (*oit)->name << "' not provided";
                }
            }
        }
    }
    if (success && !helpWrapper.showHelp) {
        for (vector<CommandGroup>::const_iterator git = groups.begin(); git != groups.end(); ++git) {
            try {
                git->cmd.runCommand();
            } catch (const std::exception &e) {
                log(LogLevel::Error) << "Error initializing commands from " << git->caption << ": " << e.what() << "\n" << Exception::getPointer();
                return false;
            }
        }
    } else {
        LogStream l = log(LogLevel::Info);
        l << "Usage: " << args.front() << " [options...]\n";
        size_t longest = 0;
        for (vector<CommandGroup>::const_iterator git = groups.begin(); git != groups.end(); ++git) {
            if (!git->caption.empty()) {
                for (vector<shared_ptr<GenericCommandParser>>::const_iterator oit = git->options.begin(); oit != git->options.end(); ++oit) {
                    longest = max(longest, (*oit)->example().length());
                }
            }
        }
        for (vector<CommandGroup>::const_iterator git = groups.begin(); git != groups.end(); ++git) {
            if (!git->caption.empty()) {
            l << "\n" << git->caption << ":\n";
                for (vector<shared_ptr<GenericCommandParser>>::const_iterator oit = git->options.begin(); oit != git->options.end(); ++oit) {
                    l << "    " << left << setw(longest) << (*oit)->example() << setw(0) << "  " << (*oit)->desc << "\n";
                }
            }
        }
    }
    return success;
}
