#ifndef SPACEPI_TARGETLIB_LINUX_EXTENSION_DIGITALIO_HPP
#define SPACEPI_TARGETLIB_LINUX_EXTENSION_DIGITALIO_HPP

#include <functional>
#include <memory>
#include <gpiod.hpp>
#include <SpacePi.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            class DigitalIOChip;

            class DigitalIO : public spacepi::resource::DigitalIO {
                public:
                    DigitalIO(std::shared_ptr<DigitalIOChip> &&chip, gpiod::line &&line);
                    ~DigitalIO();

                    operator bool();
                    spacepi::resource::DigitalIO &operator =(bool value);

                    enum Mode getMode() const noexcept;
                    void setMode(enum Mode mode);

                    const std::function<void(void)> &getISR() const noexcept;
                    enum Edge getISREdge() const noexcept;
                    void setISR(const std::function<void(void)> &isr, enum Edge edge);

                private:
                    std::shared_ptr<DigitalIOChip> chip;
                    gpiod::line line;
                    int value;
                    enum Mode mode;
                    enum Edge edge;
                    const std::function<void(void)> *isr;
            };
        }
    }
}

#endif
