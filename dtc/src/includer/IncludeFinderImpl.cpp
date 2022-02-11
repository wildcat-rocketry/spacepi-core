#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <spacepi/dtc/includer/IncludeFinder.hpp>
#include <spacepi/dtc/includer/IncludeFinderImpl.hpp>

using namespace std;
using namespace spacepi::dtc::includer;

string IncludeFinderImpl::locate(const string &filename, const vector<string> &includeDirs) const noexcept {
    for (vector<string>::const_iterator dit = includeDirs.begin(); dit != includeDirs.end(); ++dit) {
        string path = *dit + "/" + filename;
        ifstream file(path);
        if (file) {
            return path;
        }
    }

    return "";
}
