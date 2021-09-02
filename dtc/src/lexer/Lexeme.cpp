#include <ostream>
#include <stdint.h>
#include <string>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>
#include <spacepi/dtc/lexer/Lexeme.hpp>

using namespace std;
using namespace spacepi::dtc;
using namespace spacepi::dtc::diagnostics;
using namespace spacepi::dtc::lexer;

Lexeme::Lexeme(Lexeme::Type type, const SourceLocation &location) noexcept
    : location(location), type(type), intVal(0) {
}

Lexeme::Lexeme(Lexeme::Type type, const SourceLocation &location, uint32_t value) noexcept
    : location(location), type(type), intVal(value) {
}

Lexeme::Lexeme(Lexeme::Type type, const SourceLocation &location, const string &value) noexcept
    : location(location), type(type), intVal(0), strVal(value) {
}

bool Lexeme::operator==(const Lexeme &other) const noexcept {
    return location == other.location && type == other.type && intVal == other.intVal && strVal == other.strVal;
}

bool Lexeme::operator!=(const Lexeme &other) const noexcept {
    return location != other.location || type != other.type || intVal != other.intVal || strVal != other.strVal;
}

const SourceLocation &Lexeme::getLocation() const noexcept {
    return location;
}

Lexeme::Type Lexeme::getType() const noexcept {
    return type;
}

uint32_t Lexeme::getIntegerValue() const noexcept {
    return intVal;
}

const string &Lexeme::getStringValue() const noexcept {
    return strVal;
}

ostream &lexer::operator<<(ostream &os, Lexeme::Type obj) noexcept {
    switch (obj) {
        case Lexeme::VersionTag:
            return os << "VersionTag";
        case Lexeme::Semicolon:
            return os << "Semicolon";
        case Lexeme::RootNode:
            return os << "RootNode";
        case Lexeme::BraceOpen:
            return os << "BraceOpen";
        case Lexeme::BraceClose:
            return os << "BraceClose";
        case Lexeme::LabelName:
            return os << "LabelName";
        case Lexeme::Colon:
            return os << "Colon";
        case Lexeme::NodeName:
            return os << "NodeName";
        case Lexeme::PropertyName:
            return os << "PropertyName";
        case Lexeme::Equals:
            return os << "Equals";
        case Lexeme::AngleOpen:
            return os << "AngleOpen";
        case Lexeme::AngleClose:
            return os << "AngleClose";
        case Lexeme::IntLiteral:
            return os << "IntLiteral";
        case Lexeme::LabelReference:
            return os << "LabelReference";
        case Lexeme::StringLiteral:
            return os << "StringLiteral";
        case Lexeme::BracketOpen:
            return os << "BracketOpen";
        case Lexeme::BracketClose:
            return os << "BracketClose";
        case Lexeme::ByteLiteral:
            return os << "ByteLiteral";
        case Lexeme::LabelPath:
            return os << "LabelPath";
        case Lexeme::IncludeTag:
            return os << "IncludeTag";
        default:
            return os << "(unknown Lexeme::Type)";
    }
}

ostream &lexer::operator<<(ostream &os, const Lexeme &obj) noexcept {
    return os << "Lexeme(" << obj.getLocation() << ", " << obj.getType() << ", " << obj.getIntegerValue() << ", \"" << obj.getStringValue() << "\")";
}
