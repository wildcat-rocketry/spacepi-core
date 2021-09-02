#include <memory>
#include <string>
#include <spacepi/dtc/diagnostics/Diagnostic.hpp>
#include <spacepi/dtc/diagnostics/DiagnosticReporter.hpp>
#include <spacepi/dtc/diagnostics/DiagnosticReporterImpl.hpp>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>

using namespace std;
using namespace spacepi::dtc::diagnostics;

shared_ptr<DiagnosticReporter> DiagnosticReporter::instance(new DiagnosticReporterImpl());

DiagnosticReporter::ReportStream::ReportStream(DiagnosticReporter *reporter, Diagnostic::Severity severity, const SourceLocation &location) noexcept
    : reporter(reporter), severity(severity), location(location) {
}

DiagnosticReporter::ReportStream::ReportStream(const ReportStream &copy) noexcept {
}

DiagnosticReporter::ReportStream &DiagnosticReporter::ReportStream::operator=(const DiagnosticReporter::ReportStream &copy) noexcept {
    return *this;
}

DiagnosticReporter::ReportStream::~ReportStream() noexcept {
    string message = str();
    if (!message.empty()) {
        reporter->report(Diagnostic(severity, message, location));
    }
}

DiagnosticReporter::ReportStream DiagnosticReporter::info(const SourceLocation &location) noexcept {
    return ReportStream(this, Diagnostic::Info, location);
}

DiagnosticReporter::ReportStream DiagnosticReporter::warn(const SourceLocation &location) noexcept {
    return ReportStream(this, Diagnostic::Warning, location);
}

DiagnosticReporter::ReportStream DiagnosticReporter::error(const SourceLocation &location) noexcept {
    return ReportStream(this, Diagnostic::Error, location);
}
