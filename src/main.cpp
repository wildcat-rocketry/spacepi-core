#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
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
    ifstream ifs(filename);
    string content(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
    ifs.close();
    if (umount(filename.c_str()) < 0) {
        cerr << "Failure to umount " << filename << ": " << strerror(errno) << "!" << endl;
    }
    ofstream ofs(filename);
    ofs << content;
    ofs.close();
}
