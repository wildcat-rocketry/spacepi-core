#include <ostream>
#include <string>
#include <spacepi/dtc/diagnostics/Diagnostic.hpp>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>

using namespace std;
using namespace spacepi::dtc;
using namespace spacepi::dtc::diagnostics;

Diagnostic::Diagnostic(Severity severity, const string &message, const SourceLocation &location) noexcept
    : severity(severity), message(message), location(location) {
}

bool Diagnostic::operator==(const Diagnostic &other) const noexcept {
    return severity == other.severity && message == other.message && location == other.location;
}

bool Diagnostic::operator!=(const Diagnostic &other) const noexcept {
    return severity != other.severity || message != other.message || location != other.location;
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

ostream &diagnostics::operator<<(ostream &os, Diagnostic::Severity severity) noexcept {
    switch (severity) {
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
    return os << "Diagnostic(" << obj.getSeverity() << ", \"" << obj.getMessage() << "\", " << obj.getLocation() << ")";
}
