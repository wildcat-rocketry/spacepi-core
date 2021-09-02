#include <memory>
#include <vector>
#include <spacepi/dtc/diagnostics/Diagnostic.hpp>
#include <spacepi/dtc/diagnostics/DiagnosticReporter.hpp>
#include <spacepi/test/dtc/MockDiagnosticReporter.hpp>

using namespace std;
using namespace spacepi::dtc::diagnostics;
using namespace spacepi::test::dtc;

MockDiagnosticReporter::MockDiagnosticReporter() noexcept
    : old(DiagnosticReporter::instance) {
    DiagnosticReporter::instance.reset(new Impl(this));
}

MockDiagnosticReporter::~MockDiagnosticReporter() noexcept {
    DiagnosticReporter::instance = old;
}

const vector<Diagnostic> &MockDiagnosticReporter::getDiagnostics() const noexcept {
    return diags;
}

MockDiagnosticReporter::Impl::Impl(MockDiagnosticReporter *mock) noexcept
    : mock(mock) {
}

void MockDiagnosticReporter::Impl::report(const Diagnostic &diag) noexcept {
    mock->diags.push_back(diag);
}
