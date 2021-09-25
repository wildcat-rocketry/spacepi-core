#include <ostream>
#include <string>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>
#include <spacepi/dtc/tokenizer/Token.hpp>

using namespace std;
using namespace spacepi::dtc;
using namespace spacepi::dtc::diagnostics;
using namespace spacepi::dtc::tokenizer;

Token::Token(const SourceLocation &location, const string &value) noexcept
    : location(location), value(value) {
}

bool Token::operator==(const Token &other) const noexcept {
    return location == other.location && value == other.value;
}

bool Token::operator!=(const Token &other) const noexcept {
    return location != other.location || value != other.value;
}

const SourceLocation &Token::getLocation() const noexcept {
    return location;
}

const string &Token::getValue() const noexcept {
    return value;
}

ostream &tokenizer::operator<<(ostream &os, const Token &obj) noexcept {
    return os << "Token(" << obj.getLocation() << ", \"" << obj.getValue() << "\")";
}
