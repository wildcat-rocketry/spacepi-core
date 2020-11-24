#ifndef SPACEPI_EXAMPLE_CUSTOMTYPE_HPP
#define SPACEPI_EXAMPLE_CUSTOMTYPE_HPP

#include <ostream>

namespace spacepi {
    namespace example {
        class CustomType {
            public:
                CustomType() noexcept = default;
                CustomType(int val) noexcept;

                int getVal() const noexcept;

            private:
                int val;
        };
    }
}

std::ostream &operator <<(std::ostream &os, const spacepi::example::CustomType &obj);

#endif
