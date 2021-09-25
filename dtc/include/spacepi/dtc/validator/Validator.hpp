#ifndef SPACEPI_CORE_DTC_VALIDATOR_VALIDATOR_HPP
#define SPACEPI_CORE_DTC_VALIDATOR_VALIDATOR_HPP

#include <memory>
#include <spacepi/dtc/parser/SyntaxTree.hpp>

namespace spacepi {
    namespace dtc {
        namespace validator {
            /**
             * \brief Base class for the validator layer logic
             *
             * This layer is responsible for validating the device tree against a schema.
             */
            class Validator {
                public:
                    /**
                     * \brief Destroys a Validator
                     */
                    virtual ~Validator() noexcept = default;

                    /**
                     * \brief Validates a device tree against a schema
                     *
                     * \param[in] check The device tree to check
                     * \param[in] schema The schema to check against
                     * \return If it was successful
                     */
                    virtual bool validate(const parser::SyntaxTree &check, const parser::SyntaxTree &schema) noexcept = 0;

                    /**
                     * The instance of the Validator layer
                     */
                    static std::shared_ptr<Validator> instance;
            };
        }
    }
}

#endif
