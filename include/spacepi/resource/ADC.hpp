#ifndef SPACEPI_CORE_RESOURCE_ADC_HPP
#define SPACEPI_CORE_RESOURCE_ADC_HPP

#include <memory>
#include <string>

namespace spacepi {
    namespace resource {
        class ADC {
            public:
                static std::shared_ptr<ADC> get(const std::string &name);

                virtual operator double() const noexcept = 0;
        };
    }
}

#endif
