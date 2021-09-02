#ifndef SPACEPI_CORE_DTC_DIAGNOSTICS_DIAGNOSTICREPORTERIMPL_HPP
#define SPACEPI_CORE_DTC_DIAGNOSTICS_DIAGNOSTICREPORTERIMPL_HPP

#include <spacepi/dtc/diagnostics/Diagnostic.hpp>
#include <spacepi/dtc/diagnostics/DiagnosticReporter.hpp>

namespace spacepi {
    namespace dtc {
        namespace diagnostics {
            /**
             * \brief Implementation of the diagnostics layer
             */
            class DiagnosticReporterImpl : public DiagnosticReporter {
                public:
                    /**
                     * \brief Reports a Diagnostic
                     *
                     * \param[in] diag The Diagnostic
                     */
                    void report(const Diagnostic &diag) noexcept;
            };
        }
    }
}

#endif
