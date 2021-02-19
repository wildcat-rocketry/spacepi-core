#ifndef SPACEPI_TARGETLIB_LINUX_EXTENSION_DIGITALIOCHIP_HPP
#define SPACEPI_TARGETLIB_LINUX_EXTENSION_DIGITALIOCHIP_HPP

#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <unordered_map>
#include <gpiod.hpp>
#include <spacepi/target/extension/DigitalIO.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            class DigitalIOFactory;

            class DigitalIOChip : public std::enable_shared_from_this<DigitalIOChip> {
                public:
                    DigitalIOChip(DigitalIOFactory &factory, const std::string &devName);
                    ~DigitalIOChip();

                    DigitalIOChip(DigitalIOChip &) = delete;
                    DigitalIOChip &operator =(DigitalIOChip &) = delete;

                    DigitalIOFactory &getFactory() noexcept;

                    std::shared_ptr<DigitalIO> addLine(const std::string &name);
                    void removeLine(const gpiod::line &line);

                    void addInterrupt(const gpiod::line &line);
                    void removeInterrupt(const gpiod::line &line);

                private:
                    void irqPoll();

                    DigitalIOFactory &factory;
                    std::string devName;
                    std::mutex mtx;
                    bool running;
                    gpiod::chip chip;
                    gpiod::line_bulk lines;
                    std::vector<std::weak_ptr<DigitalIO>> resources;
                    std::unordered_map<std::string, int> resourceIDs;
                    std::thread irqThread;
            };
        }
    }
}

#endif
