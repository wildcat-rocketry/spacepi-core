#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
#include <spacepi/dtc/diagnostics/Diagnostic.hpp>
#include <spacepi/dtc/diagnostics/DiagnosticReporterImpl.hpp>
#include <spacepi/dtc/diagnostics/SourceFile.hpp>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>

using namespace std;
using namespace spacepi::dtc::diagnostics;

#define DEBUG DiagnosticReporter::instance->debug(Diagnostic::Diagnostics)

static const char *severities[] = {
    "debug",
    "info",
    "warning",
    "error"
};

static const char *colors[] = {
    "\033[36m",
    "\033[0m",
    "\033[33m",
    "\033[31m"
};

static ostream *streams[] = {
    &cout,
    &cout,
    &cerr,
    &cerr
};

static const char *sources[] = {
    "diagnostics",
    "emitter",
    "includer",
    "lexer",
    "main",
    "parser",
    "phandler",
    "tokenizer",
    "validator"
};

void DiagnosticReporterImpl::report(const Diagnostic &diag) noexcept {
    const char *severity = severities[diag.getSeverity()];
    const char *color = colors[diag.getSeverity()];
    ostream &os = *streams[diag.getSeverity()];
    const char *source = sources[diag.getSource()];
    if (SourceLocation loc = diag.getLocation(); loc) {
        if (shared_ptr<SourceFile> file = loc.getFile(); file) {
            if (const vector<string> &lines = file->getLines(); lines.size() >= loc.getLineNumber()) {
                int colA = loc.getColumnStart(), colB = loc.getColumnEnd();
                if (const string &line = lines[loc.getLineNumber() - 1]; colA > 0 && colA <= line.size() && colB > 0 && colB <= line.size()) {
                    os << color << source << ": " << severity << ": " << diag.getMessage() << " (" << file->getFilename() << ":" << loc.getLineNumber() << ":" << colA << ")"<< colors[Diagnostic::Info] << endl
                       << line << endl
                       << string(colA - 1, ' ') << color << string(colB - colA + 1, '~') << colors[Diagnostic::Info] << endl
                       << endl;
                } else {
                    os << color << source << ": " << severity << ": " << diag.getMessage() << " (" << file->getFilename() << ":" << loc.getLineNumber() << ":" << colA << ")"<< colors[Diagnostic::Info] << endl
                       << endl;
                    DEBUG << "Column bounds [" << colA << ", " << colB << "] not found for line in " << file->getFilename() << ":" << loc.getLineNumber();
                }
            } else {
                os << color << source << ": " << severity << ": " << diag.getMessage() << " (" << file->getFilename() << ":" << loc.getLineNumber() << ")"<< colors[Diagnostic::Info] << endl
                   << endl;
                DEBUG << "Line " << loc.getLineNumber() << " not found in " << file->getFilename();
            }
            return;
        }
    }
    os << color << source << ": " << severity << ": " << diag.getMessage() << colors[Diagnostic::Info] << endl
       << endl;
}
