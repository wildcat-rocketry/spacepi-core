#ifndef SPACEPI_CORE_DTC_VALIDATOR_VALIDATORIMPL_HPP
#define SPACEPI_CORE_DTC_VALIDATOR_VALIDATORIMPL_HPP

#include <spacepi/dtc/parser/SyntaxTree.hpp>
#include <spacepi/dtc/validator/Validator.hpp>

namespace spacepi {
    namespace dtc {
        namespace validator {
            /**
             * \brief Implementation of the validator layer
             */
            class ValidatorImpl : public Validator {
                public:
                    /**
                     * \brief Validates a device tree against a schema
                     *
                     * \param[in] check The device tree to check
                     * \param[in] schema The schema to check against
                     * \return If it was successful
                     */
                    bool validate(const parser::SyntaxTree &check, const parser::SyntaxTree &schema) noexcept;
            };
        }
    }
}

#endif
