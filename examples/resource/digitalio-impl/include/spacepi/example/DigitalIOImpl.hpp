#ifndef SPACEPI_EXAMPLE_DIGITALIOIMPL_HPP
#define SPACEPI_EXAMPLE_DIGITALIOIMPL_HPP

#include <functional>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/resource/DigitalIO.hpp>

namespace spacepi {
    namespace example {
        class DigitalIOImpl : public spacepi::resource::DigitalIO, private spacepi::log::AutoLog<decltype("example:impl"_autolog)> {
            public:
                operator bool();
                DigitalIO &operator =(bool value);

                enum Mode getMode() const noexcept;
                void setMode(enum Mode mode);

                const std::function<void(void)> &getISR() const noexcept;
                enum Edge getISREdge() const noexcept;
                void setISR(const std::function<void(void)> &isr, enum Edge edge);
                void clearISR();
        };
    }
}

#endif
