#include <vector>
#include <gtest/gtest.h>
#include <spacepi/dtc/diagnostics/Diagnostic.hpp>
#include <spacepi/dtc/main/Main.hpp>
#include <spacepi/test/dtc/MockDiagnosticReporter.hpp>

using namespace std;
using namespace spacepi::dtc::diagnostics;
using namespace spacepi::dtc::main;
using namespace spacepi::test::dtc;

TEST(dtc_main, argumentParsing) {
    vector<const char *> args;
    args.push_back("spacepi-dtc");
    args.push_back("--crash");
    MockDiagnosticReporter diags;
    Main::instance->run(2, args.data());
    EXPECT_EQ(diags.getDiagnostics().size(), 1);
    if (diags.getDiagnostics().size() >= 1) {
        EXPECT_EQ(diags.getDiagnostics()[0], Diagnostic(Diagnostic::Error, "Unknown option '--crash'"));
    }
}
