#include <cstring>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <boost/filesystem.hpp>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/util/System.hpp>
#include <spacepi/spacepictl/Config.hpp>
#include <spacepi/spacepictl/Exec.hpp>
#include <spacepi/spacepictl/Verb.hpp>

using namespace std;
using namespace spacepi::log;
using namespace spacepi::package;
using namespace spacepi::spacepictl;
using namespace spacepi::spacepictl::util;
namespace fs = boost::filesystem;

Exec Exec::instance;

Exec::Exec() noexcept : Verb("exec", "Execute a SpacePi service") {
}

bool Exec::run(const vector<string> &args) {
    if (args.size() != 1) {
        printErrorHelp("Incorrect number of arguments.");
        return false;
    }

    PackageConfig cfg(SPACEPI_RUNNING_CONF);
    unordered_map<string, Module>::iterator it = cfg.getModules().find(args.front());
    if (it == cfg.getModules().end()) {
        printErrorHelp("Module \"" + args.front() + "\" not found");
        return false;
    }

    vector<char*> modArgs;
    vector<string> str_args;
    vector<vector<char>> alloc;
    string executable = System::moduleBin(it->second);
    modArgs.push_back(doAlloc(alloc, executable));

    Options options = it->second.getOptions();
    for( const auto& option : options){
        str_args.push_back("--" + option.getKey() + "=" + option.getValue());
    }

    for(const auto &arg : str_args){
        modArgs.push_back(doAlloc(alloc, arg));
    }
    modArgs.push_back(nullptr);

    vector<char*> env;
    ostringstream ld_preload;
    ld_preload << "LD_PRELOAD=";
    fs::path libDir = fs::read_symlink("/proc/self/exe").parent_path().parent_path() / "lib";
    for (fs::directory_iterator dit(libDir); dit != fs::directory_iterator(); ++dit) {
        fs::path child = dit->path();
        if (child.filename().native().substr(0, 15) == "libspacepi-ext-" && child.extension() == "so") {
            ld_preload << child.normalize().native() << " ";
        }
    }
    ld_preload.seekp(-1, ios_base::cur);
    env.push_back(doAlloc(alloc, ld_preload.str()));
    env.push_back(nullptr);

    execvpe(executable.c_str(), modArgs.data(), env.data());

    LogStream ss = log(LogLevel::Error);
    ss << "Error executing";
    for(auto &arg : args) ss << " " << arg;

    return false;
}

ostream &Exec::printHelp(ostream &os) const noexcept {
    return os << " <name>\n"
                 "Options:\n"
                 "    name  The name of the service unit to exec\n";
}

char *Exec::doAlloc(vector<vector<char>> &alloc, const string &str) {
    alloc.emplace_back(str.size() + 1);
    char *buf = alloc.back().data();
    strcpy(buf, str.data());
    return buf;
}
