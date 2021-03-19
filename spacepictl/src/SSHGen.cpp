#include <ostream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <SpacePi.hpp>
#include <spacepi/spacepictl/util/RWRoot.hpp>
#include <spacepi/spacepictl/SSHGen.hpp>
#include <spacepi/spacepictl/Verb.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace boost::process;
using namespace spacepi::log;
using namespace spacepi::spacepictl;
using namespace spacepi::spacepictl::util;

SSHGen SSHGen::instance;

SSHGen::SSHGen() noexcept : Verb("ssh-gen", "Generates missing SSH keys for the system", true), success(true) {
}

bool SSHGen::run(const vector<string> &args) {
    RWRoot root;
    generate("/etc/ssh/id_rsa",               "rsa",     "SpacePi"     );
    generate("/etc/ssh/ssh_host_rsa_key",     "rsa",     "SpacePi-host");
    generate("/etc/ssh/ssh_host_ecdsa_key",   "ecdsa",   "SpacePi-host");
    return success;
}

void SSHGen::generate(const string &privateKey, const string &type, const string &comment) noexcept {
    if (!exists(privateKey) || !exists(privateKey + ".pub")) {
        log(LogLevel::Info) << "Generating " << privateKey << "...";
        child proc("/usr/bin/ssh-keygen", "-q", "-t", type, "-N", "", "-C", comment, "-f", privateKey);
        proc.wait();
        if (proc.exit_code() != 0) {
            log(LogLevel::Warning) << "Unable to generate key: " << privateKey;
            success = false;
        }
    }
}

ostream &SSHGen::printHelp(ostream &os) const noexcept {
    return os << "\n";
}
