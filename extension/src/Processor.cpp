#include <array>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <dirent.h>
#include <SpacePi.hpp>
#include <spacepi/target/extension/detail/UniqueDir.hpp>
#include <spacepi/target/extension/Processor.hpp>

using namespace std;
using namespace std::chrono;
using namespace spacepi;
using namespace spacepi::log;
using namespace spacepi::util;
using namespace spacepi::target::extension;
using namespace spacepi::target::extension::detail;

#define UPTIME_FILE "/proc/uptime"
#define PROC_DIR "/proc"

seconds Processor::getUptime() {
    ifstream ifs(UPTIME_FILE);
    if (!ifs) {
        throw EXCEPTION(ResourceException("Unable to open " UPTIME_FILE));
    }
    float time;
    ifs >> time;
    if (ifs.bad()) {
        throw EXCEPTION(ResourceException("Could not read " UPTIME_FILE));
    } else if (ifs.fail()) {
        throw EXCEPTION(ResourceException(UPTIME_FILE " is incorrectly formatted"));
    }
    return seconds((int) (time + 0.5f));
}

array<double, 3> Processor::getLoadAverage() {
    decltype(getLoadAverage()) arr;
    if (getloadavg(arr.data(), arr.size()) < 0) {
        throw EXCEPTION(ResourceException("Could not get load average"));
    }
    return arr;
}

resource::ProcessInfo Processor::getProcessInfo() {
    UniqueDir d = opendir(PROC_DIR);
    if (!d) {
        throw EXCEPTION(ResourceException("Unable to open " PROC_DIR));
    }
    int running = 0;
    int sleeping = 0;
    int stopped = 0;
    int zombied = 0;
    struct dirent *dir;
    while (dir = readdir(d)) {
        if (dir->d_type == DT_DIR) {
            string name = dir->d_name;
            if (name.find_first_not_of("0123456789") == string::npos) {
                string filename = PROC_DIR "/" + name + "/stat";
                ifstream ifs(filename);
                if (!ifs) {
                    throw EXCEPTION(ResourceException("Could not open " + filename));
                }
                int pid;
                string exeName;
                char state;
                ifs >> pid >> exeName >> state;
                if (ifs.bad()) {
                    throw EXCEPTION(ResourceException("Could not read " + filename));
                } else if (ifs.fail()) {
                    throw EXCEPTION(ResourceException(filename + " is incorrectly formatted"));
                }
                switch (state) {
                    case 'R':
                        ++running;
                        break;
                    case 'S':
                    case 'D':
                        ++sleeping;
                        break;
                    case 'T':
                    case 't':
                        ++stopped;
                        break;
                    case 'Z':
                    case 'X':
                        ++zombied;
                        break;
                    case 'I':
                        // kernel thread?
                        break;
                    default:
                        log(LogLevel::Warning) << "Unknown process status code '" << state << "' for process '" << exeName << "' (" << dir->d_name << ")";
                        break;
                }
            }
        }
    }
    return resource::ProcessInfo(running, sleeping, stopped, zombied);
}
