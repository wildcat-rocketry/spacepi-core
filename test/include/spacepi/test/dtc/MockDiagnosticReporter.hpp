#ifndef SPACEPI_CORE_TEST_DTC_MOCKDIAGNOSTICREPORTER_HPP
#define SPACEPI_CORE_TEST_DTC_MOCKDIAGNOSTICREPORTER_HPP

#include <memory>
#include <vector>
#include <spacepi/dtc/diagnostics/Diagnostic.hpp>
#include <spacepi/dtc/diagnostics/DiagnosticReporter.hpp>

namespace spacepi {
    namespace test {
        namespace dtc {
            class MockDiagnosticReporter {
                public:
                    MockDiagnosticReporter() noexcept;
                    ~MockDiagnosticReporter() noexcept;

                    const std::vector<spacepi::dtc::diagnostics::Diagnostic> &getDiagnostics() const noexcept;

                private:
                    class Impl : public spacepi::dtc::diagnostics::DiagnosticReporter {
                        public:
                            Impl(MockDiagnosticReporter *mock) noexcept;
                            void report(const spacepi::dtc::diagnostics::Diagnostic &diag) noexcept;

                        private:
                            MockDiagnosticReporter *mock;
                    };

                    std::vector<spacepi::dtc::diagnostics::Diagnostic> diags;
                    std::shared_ptr<spacepi::dtc::diagnostics::DiagnosticReporter> old;
            };
        }
    }
}

#endif
