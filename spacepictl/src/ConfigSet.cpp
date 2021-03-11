#include <algorithm>
#include <ostream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <spacepi/spacepictl/util/FSTransaction.hpp>
#include <spacepi/spacepictl/Config.hpp>
#include <spacepi/spacepictl/ConfigSet.hpp>
#include <spacepi/spacepictl/Verb.hpp>

using namespace std;
using namespace spacepi::log;
using namespace spacepi::spacepictl;
using namespace spacepi::spacepictl::util;
namespace fs = boost::filesystem;

ConfigSet ConfigSet::instance;

ConfigSet::ConfigSet() noexcept : Verb("config-set", "Set the next config file") {
}

bool ConfigSet::run(const vector<string> &args) {
    if (args.size() != 1) {
        printErrorHelp("Incorrect number of arguments.");
        return false;
    }

    vector<fs::path> paths = resolve_path(fs::path(args.front()));
    if(paths.size() > 1){
        log(LogLevel::Error) << "Ambiguous path. Matches:\n";
        for(const auto &match : paths){
            log(LogLevel::Error) << match.native() << "\n";
        }
        return false;
    }

    fs::path config_path;

    if(paths.size() == 1){
        config_path = paths[0];
    } else {
        vector<string> configs = resolve_config(args.front());
        if(configs.size() > 1){
            log(LogLevel::Error) << "Ambiguous config name. Matches:\n";
            for(const auto &match : configs){
                log(LogLevel::Error) << match << "\n";
            }
            return 1;
        } else if(configs.size() == 0){
            log(LogLevel::Error) << "Argument does not match any configs\n";
            return false;
        } else {
            //config_path =  fs::path(SPACEPI_CONFIGS).append(configs[0]);
            config_path =  configs[0];
        }
    }

    string config = fs::canonical(config_path).native();

    log(LogLevel::Info) << "Linking " << SPACEPI_NEW_PATH_CONF << " -> " << config << "\n";
    log(LogLevel::Info) << "New configuration will activate on next boot\n";

    FSTransaction fs;
    if(!fs::exists(SPACEPI_ETC)) fs.mkdir(SPACEPI_ETC);
    fs.link(SPACEPI_NEW_PATH_CONF, config);
    fs.apply();

    return true;
}

ostream &ConfigSet::printHelp(ostream &os) const noexcept {
    return os << " <config>\n"
                 "Options:\n"
                 "    config  The path or name of the config to use\n";
}

// Do a prefix match on the filename for a path and return matches
vector<fs::path> ConfigSet::resolve_path(const fs::path& p){
    fs::path directory = p.parent_path();
    string name = p.filename().native();
    vector<fs::path> matching_paths = {};

    if(fs::exists(directory)){
        for(const fs::path &p2 : fs::directory_iterator(directory)){
            if(!fs::is_directory(p2)){
                string name2 = p2.filename().native();
                const auto &match = std::mismatch(name.begin(), name.end(), name2.begin(), name2.end());
                if(match.first == name.end()){
                    // Given name is prefix to other path
                    if(match.second == name2.end()){
                        // Given name exactly matches given path
                        return {p2};
                    }

                    matching_paths.push_back(p2);
                }
            }
        }
    }

    return matching_paths;
}

vector<string> ConfigSet::resolve_config(const string name){
    vector<string> matches = {};
#ifdef SPACEPI_PACKAGES_FILES
        for(const fs::path &item : {SPACEPI_PACKAGES_FILES}){
            if(item.has_stem() && name == item.stem().native()){
                matches.push_back(item.native());
            } else {
                fs::path name_path(name);
                string reduced_name;
                if(name_path.has_stem()){
                    reduced_name = name_path.parent_path().append(name_path.stem().native()).native();
                } else {
                    reduced_name = name;
                }
                string item_str = item.parent_path().append(item.stem().native()).native();
                const auto &match = std::mismatch(reduced_name.rbegin(), reduced_name.rend(), item_str.rbegin(), item_str.rend());
                if(match.first == reduced_name.rend()){
                    if(match.second == item_str.rend()){
                        return {item.native()};
                    }

                    matches.push_back(item.native());
                }
            }
        }
#endif
    return matches;
}
