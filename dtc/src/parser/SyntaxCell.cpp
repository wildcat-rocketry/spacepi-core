#include <ostream>
#include <stdint.h>
#include <string>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>
#include <spacepi/dtc/parser/SyntaxCell.hpp>

using namespace std;
using namespace spacepi::dtc;
using namespace spacepi::dtc::diagnostics;
using namespace spacepi::dtc::parser;

SyntaxCell::SyntaxCell(const SourceLocation &location, uint32_t value) noexcept
    : location(location), value(value) {
}

SyntaxCell::SyntaxCell(const SourceLocation &location, const string &label) noexcept
    : location(location), value(0), label(label) {
}

bool SyntaxCell::operator==(const SyntaxCell &other) const noexcept {
    return location == other.location && value == other.value && label == other.label;
}

bool SyntaxCell::operator!=(const SyntaxCell &other) const noexcept {
    return location != other.location || value != other.value || label != other.label;
}

const SourceLocation &SyntaxCell::getLocation() const noexcept {
    return location;
}

uint32_t SyntaxCell::getValue() const noexcept {
    return value;
}

const string &SyntaxCell::getLabel() const noexcept {
    return label;
}

bool SyntaxCell::isLabel() const noexcept {
    return !label.empty();
}

ostream &parser::operator<<(ostream &os, const SyntaxCell &obj) noexcept {
    return os << "SyntaxCell(" << obj.getLocation() << ", " << obj.getValue() << ", \"" << obj.getLabel() << "\")";
}
