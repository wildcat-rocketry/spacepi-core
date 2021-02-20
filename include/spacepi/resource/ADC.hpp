#ifndef SPACEPI_CORE_RESOURCE_ADC_HPP
#define SPACEPI_CORE_RESOURCE_ADC_HPP

#include <memory>
#include <string>

namespace spacepi {
    namespace resource {
        /**
         * \brief A hardware resource which represents an analog to digital converter
         */
        class ADC {
            public:
                /**
                 * \brief Destroy this ADC resource
                 */
                virtual ~ADC() = default;

                /**
                 * \brief Get a pointer to an ADC object by its resource name
                 * 
                 * \param[in] name The resource name for which to get the ADC object
                 * 
                 * \return The ADC object, or a null pointer if the name is not registered
                 */
                static std::shared_ptr<ADC> get(const std::string &name);

                /**
                 * \brief Read the ADC value
                 * 
                 * \return The value of the ADC, on a range from 0 to 1
                 */
                virtual operator double() const noexcept = 0;
        };
    }
}

#endif
