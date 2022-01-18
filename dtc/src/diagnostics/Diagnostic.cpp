#include <ostream>
#include <string>
#include <spacepi/dtc/diagnostics/Diagnostic.hpp>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>

using namespace std;
using namespace spacepi::dtc;
using namespace spacepi::dtc::diagnostics;

Diagnostic::Diagnostic(Source source, Severity severity, const string &message, const SourceLocation &location) noexcept
    : source(source), severity(severity), message(message), location(location) {
}

bool Diagnostic::operator==(const Diagnostic &other) const noexcept {
    return source == other.source && severity == other.severity && message == other.message && location == other.location;
}

bool Diagnostic::operator!=(const Diagnostic &other) const noexcept {
    return source != other.source || severity != other.severity || message != other.message || location != other.location;
}

Diagnostic::Source Diagnostic::getSource() const noexcept {
    return source;
}

Diagnostic::Severity Diagnostic::getSeverity() const noexcept {
    return severity;
}

const string &Diagnostic::getMessage() const noexcept {
    return message;
}

const SourceLocation &Diagnostic::getLocation() const noexcept {
    return location;
}

ostream &diagnostics::operator<<(ostream &os, Diagnostic::Source source) noexcept {
    switch (source) {
        case Diagnostic::Diagnostics:
            return os << "Diagnostics";
        case Diagnostic::Emitter:
            return os << "Emitter";
        case Diagnostic::Includer:
            return os << "Includer";
        case Diagnostic::Lexer:
            return os << "Lexer";
        case Diagnostic::Main:
            return os << "Main";
        case Diagnostic::Parser:
            return os << "Parser";
        case Diagnostic::Tokenizer:
            return os << "Tokenizer";
        case Diagnostic::Validator:
            return os << "Validator";
        default:
            return os << "(unknown Diagnostic::Source)";
    }
}

ostream &diagnostics::operator<<(ostream &os, Diagnostic::Severity severity) noexcept {
    switch (severity) {
        case Diagnostic::Debug:
            return os << "Debug";
        case Diagnostic::Info:
            return os << "Info";
        case Diagnostic::Warning:
            return os << "Warning";
        case Diagnostic::Error:
            return os << "Error";
        default:
            return os << "(unknown Diagnostic::Severity)";
    }
}

ostream &diagnostics::operator<<(ostream &os, const Diagnostic &obj) noexcept {
    return os << "Diagnostic(" << obj.getSource() << ", " << obj.getSeverity() << ", \"" << obj.getMessage() << "\", " << obj.getLocation() << ")";
}
