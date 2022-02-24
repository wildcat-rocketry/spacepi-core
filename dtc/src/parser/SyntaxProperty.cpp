#include <ostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>
#include <spacepi/dtc/parser/SyntaxCell.hpp>
#include <spacepi/dtc/parser/SyntaxProperty.hpp>

using namespace std;
using namespace spacepi::dtc;
using namespace spacepi::dtc::diagnostics;
using namespace spacepi::dtc::parser;

SyntaxProperty::SyntaxProperty(const SourceLocation &location, const string &name, const vector<SyntaxCell> &cells) noexcept
    : location(location), name(name), cells(cells) {
}

SyntaxProperty::SyntaxProperty(const SourceLocation &location, const string &name, const vector<uint8_t> &bytes) noexcept
    : location(location), name(name), bytes(bytes) {
}

SyntaxProperty::SyntaxProperty(const SourceLocation &location, const string &name, const string &label) noexcept
    : location(location), name(name), label(label) {
}

vector<uint8_t> SyntaxProperty::literalStringBytes(const string &str) noexcept {
    vector<uint8_t> vec;
    vec.reserve(str.size());
    for (string::const_iterator it = str.begin(); it != str.end(); ++it) {
        vec.push_back((uint8_t) *it);
    }
    return vec;
}

bool SyntaxProperty::operator==(const SyntaxProperty &other) const noexcept {
    if (location != other.location || name != other.name || label != other.name) {
        return false;
    }
    vector<SyntaxCell>::const_iterator it1 = cells.begin();
    vector<SyntaxCell>::const_iterator it2 = other.cells.begin();
    while (it1 != cells.end() && it2 != other.cells.end()) {
        if (*it1++ == *it2++) {
            return false;
        }
    }
    vector<uint8_t>::const_iterator it3 = bytes.begin();
    vector<uint8_t>::const_iterator it4 = other.bytes.begin();
    while (it3 != bytes.end() && it4 != other.bytes.end()) {
        if (*it3++ == *it4++) {
            return false;
        }
    }
    return it1 == cells.end() && it2 == other.cells.end() && it3 == bytes.end() && it4 == other.bytes.end();
}

bool SyntaxProperty::operator!=(const SyntaxProperty &other) const noexcept {
    if (location != other.location || name != other.name || label != other.label) {
        return true;
    }
    vector<SyntaxCell>::const_iterator it1 = cells.begin();
    vector<SyntaxCell>::const_iterator it2 = other.cells.begin();
    while (it1 != cells.end() && it2 != other.cells.end()) {
        if (*it1++ != *it2++) {
            return true;
        }
    }
    vector<uint8_t>::const_iterator it3 = bytes.begin();
    vector<uint8_t>::const_iterator it4 = other.bytes.begin();
    while (it3 != bytes.end() && it4 != other.bytes.end()) {
        if (*it3++ != *it4++) {
            return true;
        }
    }
    return it1 != cells.end() || it2 != other.cells.end() || it3 != bytes.end() || it4 != other.bytes.end();
}

const SourceLocation &SyntaxProperty::getLocation() const noexcept {
    return location;
}

const string &SyntaxProperty::getName() const noexcept {
    return name;
}

const vector<SyntaxCell> &SyntaxProperty::getCells() const noexcept {
    return cells;
}

const vector<uint8_t> &SyntaxProperty::getBytes() const noexcept {
    return bytes;
}

const string &SyntaxProperty::getLabel() const noexcept {
    return label;
}

bool SyntaxProperty::isString() const noexcept {
    return !bytes.empty() || isLabel();
}

bool SyntaxProperty::isLabel() const noexcept {
    return !label.empty();
}

ostream &parser::operator<<(ostream &os, const SyntaxProperty &obj) noexcept {
    return os << "SyntaxProperty(" << obj.getLocation() << ", \"" << obj.getName() << "\", \"" << obj.getLabel() << "\")";
}
