#include <memory>
#include <string>
#include <spacepi/dtc/diagnostics/Diagnostic.hpp>
#include <spacepi/dtc/diagnostics/DiagnosticReporter.hpp>
#include <spacepi/dtc/diagnostics/DiagnosticReporterImpl.hpp>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>

using namespace std;
using namespace spacepi::dtc::diagnostics;

shared_ptr<DiagnosticReporter> DiagnosticReporter::instance(new DiagnosticReporterImpl());

DiagnosticReporter::ReportStream::ReportStream(DiagnosticReporter *reporter, Diagnostic::Source source, Diagnostic::Severity severity, const SourceLocation &location) noexcept
    : reporter(reporter), source(source), severity(severity), location(location) {
}

DiagnosticReporter::ReportStream::ReportStream(const ReportStream &copy) noexcept {
}

DiagnosticReporter::ReportStream &DiagnosticReporter::ReportStream::operator=(const DiagnosticReporter::ReportStream &copy) noexcept {
    return *this;
}

DiagnosticReporter::ReportStream::~ReportStream() noexcept {
    string message = str();
    if (!message.empty()) {
        reporter->report(Diagnostic(source, severity, message, location));
    }
}

DiagnosticReporter::ReportStream DiagnosticReporter::debug(Diagnostic::Source source, const SourceLocation &location) noexcept {
    return ReportStream(this, source, Diagnostic::Debug, location);
}

DiagnosticReporter::ReportStream DiagnosticReporter::info(Diagnostic::Source source, const SourceLocation &location) noexcept {
    return ReportStream(this, source, Diagnostic::Info, location);
}

DiagnosticReporter::ReportStream DiagnosticReporter::warn(Diagnostic::Source source, const SourceLocation &location) noexcept {
    return ReportStream(this, source, Diagnostic::Warning, location);
}

DiagnosticReporter::ReportStream DiagnosticReporter::error(Diagnostic::Source source, const SourceLocation &location) noexcept {
    return ReportStream(this, source, Diagnostic::Error, location);
}
