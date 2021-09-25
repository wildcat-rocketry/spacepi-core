#include <ostream>
#include <string>
#include <vector>
#include <spacepi/dtc/diagnostics/SourceFile.hpp>

using namespace std;
using namespace spacepi::dtc;
using namespace spacepi::dtc::diagnostics;

SourceFile::SourceFile(const string &filename, const vector<string> &lines) noexcept
    : filename(filename), lines(lines) {
}

bool SourceFile::operator==(const SourceFile &other) const noexcept {
    if (this == &other) {
        return true;
    }
    if (filename != other.filename) {
        return false;
    }
    vector<string>::const_iterator it1 = lines.begin();
    vector<string>::const_iterator it2 = other.lines.begin();
    while (it1 != lines.end() && it2 != other.lines.end()) {
        if (*it1 != *it2) {
            return false;
        }
    }
    return it1 == lines.end() && it2 == other.lines.end();
}

bool SourceFile::operator!=(const SourceFile &other) const noexcept {
    if (this == &other) {
        return false;
    }
    if (filename != other.filename) {
        return true;
    }
    vector<string>::const_iterator it1 = lines.begin();
    vector<string>::const_iterator it2 = other.lines.begin();
    while (it1 != lines.end() && it2 != other.lines.end()) {
        if (*it1++ != *it2++) {
            return true;
        }
    }
    return it1 != lines.end() || it2 != other.lines.end();
}

const string &SourceFile::getFilename() const noexcept {
    return filename;
}

const vector<string> &SourceFile::getLines() const noexcept {
    return lines;
}

ostream &diagnostics::operator<<(ostream &os, const SourceFile &obj) noexcept {
    return os << "SourceFile(" << obj.getFilename() << ")";
}
