#include <iostream>
#include <spacepi/dtc/diagnostics/Diagnostic.hpp>
#include <spacepi/dtc/diagnostics/DiagnosticReporterImpl.hpp>

using namespace std;
using namespace spacepi::dtc::diagnostics;

void DiagnosticReporterImpl::report(const Diagnostic &diag) noexcept {
    // TODO
    cout << diag << endl;
}
