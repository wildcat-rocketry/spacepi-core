#include <cerrno>
#include <cstring>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <boost/filesystem.hpp>
#include <SpacePi.hpp>
#include <spacepi/liblinux/steps/EnsureRootStep.hpp>
#include <spacepi/liblinux/Config.hpp>
#include <spacepi/liblinux/InstallationData.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace spacepi::log;
using namespace spacepi::util;
using namespace spacepi::liblinux;
using namespace spacepi::liblinux::steps;

void EnsureRootStep::run(InstallationData &data) {
    if (getuid() == 0) {
        return;
    }
    log(LogLevel::Error) << "SpacePi installer requires root privileges.\n"
#ifdef SUDO_EXECUTABLE
        "Trying sudo...";
#elif defined(SU_EXECUTABLE)
        "Trying su...";
#else
        "Unable to find sudo or su.  Exiting.";
#endif
#if defined(SUDO_EXECUTABLE) || defined(SU_EXECUTABLE)
    vector<vector<char>> args;
#ifdef SUDO_EXECUTABLE
    args.push_back(vector<char> { 's', 'u', 'd', 'o', '\0' });
#else
    args.push_back(vector<char> { 's', 'u', '\0' });
    args.push_back(vector<char> { '-', 'c', '\0' });
#endif
    string line;
    std::ifstream argFile("/proc/self/cmdline");
    while (getline(argFile, line, '\0')) {
        if (!line.empty()) {
            args.emplace_back();
            args.back().resize(line.size() + 1);
            strcpy(args.back().data(), line.data());
        }
    }
    argFile.close();
    vector<char *> argv;
    argv.reserve(args.size());
    for (vector<vector<char>>::iterator it = args.begin(); it != args.end(); ++it) {
        argv.push_back(it->data());
    }
    argv.push_back(nullptr);
#ifdef SUDO_EXECUTABLE
    execv(SUDO_EXECUTABLE, argv.data());
    log(LogLevel::Error) << "Failed to exec " SUDO_EXECUTABLE ": " << strerror(errno)
#ifdef SU_EXECUTABLE
        << "\nTrying su...";
    args.front() = { 's', 'u', '\0' };
    argv.front() = args.front().data();
    args.push_back(vector<char> { '-', 'c', '\0' });
    argv.insert(argv.begin() + 1, args.back().data());
#else
        ;
#endif
#endif
#ifdef SU_EXECUTABLE
    execv(SU_EXECUTABLE, argv.data());
    log(LogLevel::Error) << "Failed to exec " SU_EXECUTABLE ": " << strerror(errno);
#endif
#endif
    throw EXCEPTION(ResourceException("Process needs root privileges."));
}
