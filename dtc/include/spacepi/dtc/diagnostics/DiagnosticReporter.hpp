#ifndef SPACEPI_CORE_DTC_DIAGNOSTICS_DIAGNOSTICREPORTER_HPP
#define SPACEPI_CORE_DTC_DIAGNOSTICS_DIAGNOSTICREPORTER_HPP

#include <memory>
#include <sstream>
#include <spacepi/dtc/diagnostics/Diagnostic.hpp>
#include <spacepi/dtc/diagnostics/SourceLocation.hpp>

namespace spacepi {
    namespace dtc {
        namespace diagnostics {
            /**
             * \brief Base class for the diagnostics layer logic
             *
             * This layer is responsible for reporting diagnostic information to the user from other layers
             */
            class DiagnosticReporter {
                public:
                    /**
                     * \private
                     */
                    class ReportStream : public std::ostringstream {
                        public:
                            ReportStream(DiagnosticReporter *reporter, Diagnostic::Source source, Diagnostic::Severity severity, const SourceLocation &location) noexcept;
                            ReportStream(const ReportStream &copy) noexcept;
                            ReportStream &operator=(const ReportStream &copy) noexcept;
                            ~ReportStream();

                        private:
                            DiagnosticReporter *reporter;
                            Diagnostic::Source source;
                            Diagnostic::Severity severity;
                            SourceLocation location;
                    };

                    /**
                     * \brief Destroys a DiagnosticReporter
                     */
                    virtual ~DiagnosticReporter() noexcept = default;

                    /**
                     * \brief Reports a Diagnostic
                     *
                     * \param[in] diag The Diagnostic
                     */
                    virtual void report(const Diagnostic &diag) noexcept = 0;

                    /**
                     * \brief Reports an debugging Diagnostic
                     *
                     * \param[in] source The source of the Diagnostic
                     * \param[in] location The SourceLocation which contains the problem
                     * \return A stream to write the message to
                     */
                    ReportStream debug(Diagnostic::Source source, const SourceLocation &location = SourceLocation()) noexcept;

                    /**
                     * \brief Reports an informational Diagnostic
                     *
                     * \param[in] source The source of the Diagnostic
                     * \param[in] location The SourceLocation which contains the problem
                     * \return A stream to write the message to
                     */
                    ReportStream info(Diagnostic::Source source, const SourceLocation &location = SourceLocation()) noexcept;

                    /**
                     * \brief Reports a warning Diagnostic
                     *
                     * \param[in] source The source of the Diagnostic
                     * \param[in] location The SourceLocation which contains the problem
                     * \return A stream to write the message to
                     */
                    ReportStream warn(Diagnostic::Source source, const SourceLocation &location = SourceLocation()) noexcept;

                    /**
                     * \brief Reports an error Diagnostic
                     *
                     * \param[in] source The source of the Diagnostic
                     * \param[in] location The SourceLocation which contains the problem
                     * \return A stream to write the message to
                     */
                    ReportStream error(Diagnostic::Source source, const SourceLocation &location = SourceLocation()) noexcept;

                    /**
                     * \brief The instance of the DiagnosticReporter layer
                     */
                    static std::shared_ptr<DiagnosticReporter> instance;
            };
        }
    }
}

#endif
