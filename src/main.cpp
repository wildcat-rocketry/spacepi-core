#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/mount.h>

using namespace std;

static void fixFile(const std::string &filename);

int main() {
    fixFile("/etc/hosts");
    fixFile("/etc/hostname");
    execl("/usr/local/bin/spacepictl", "/usr/local/bin/spacepictl", nullptr);
    cerr << "Failure to exec spacepictl: " << strerror(errno) << "!" << endl;
    return 1;
}

static void fixFile(const std::string &filename) {
    ifstream ifs(filename, ios::binary | ios::ate);
    ifstream::pos_type pos = ifs.tellg();
    vector<char> content(pos);
    ifs.seekg(0, ios::beg);
    ifs.read(&content[0], pos);
    ifs.close();

    if (umount(filename.c_str()) < 0) {
        cerr << "Failure to umount " << filename << ": " << strerror(errno) << "!" << endl;
    }

    ofstream ofs(filename);
    for (const auto &e : content) ofs << e;
    ofs.close();
}
