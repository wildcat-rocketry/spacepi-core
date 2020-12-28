#ifndef SPACEPI_CORE_RESOURCE_DIGITALIO_HPP
#define SPACEPI_CORE_RESOURCE_DIGITALIO_HPP

#include <functional>
#include <memory>
#include <ostream>
#include <string>

namespace spacepi {
    namespace resource {
        class DigitalIO {
            public:
                enum Mode {
                    Output = 0,
                    Input = 1,
                    OpenDrain = 2,
                    OpenSource = 4,
                    PullUp = 2,
                    PullDown = 4
                };

                enum Edge {
                    Disable = 0,
                    Rising = 1,
                    Falling = 2,
                    Transition = 3
                };

                static std::shared_ptr<DigitalIO> get(const std::string &name);

                virtual operator bool() = 0;
                virtual DigitalIO &operator =(bool value) = 0;

                virtual enum Mode getMode() const noexcept = 0;
                virtual void setMode(enum Mode mode) = 0;

                virtual const std::function<void(void)> &getISR() const noexcept = 0;
                virtual enum Edge getISREdge() const noexcept = 0;
                virtual void setISR(const std::function<void(void)> &isr, enum Edge edge) = 0;
                void clearISR();
        };
    }
}

enum spacepi::resource::DigitalIO::Mode operator |(enum spacepi::resource::DigitalIO::Mode a, enum spacepi::resource::DigitalIO::Mode b) noexcept;
enum spacepi::resource::DigitalIO::Mode &operator |=(enum spacepi::resource::DigitalIO::Mode &a, enum spacepi::resource::DigitalIO::Mode b) noexcept;

std::ostream &operator <<(std::ostream &os, spacepi::resource::DigitalIO::Mode mode);
std::ostream &operator <<(std::ostream &os, spacepi::resource::DigitalIO::Edge edge);

#endif
