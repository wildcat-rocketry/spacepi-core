#ifndef SPACEPI_TARGETLIB_LINUX_EXTENSION_DIGITALIOFACTORY_HPP
#define SPACEPI_TARGETLIB_LINUX_EXTENSION_DIGITALIOFACTORY_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <SpacePi.hpp>
#include <spacepi/target/extension/DigitalIOChip.hpp>

namespace spacepi {
    namespace target {
        namespace extension {
            class DigitalIOFactory : public spacepi::resource::ResourceFactory<spacepi::resource::DigitalIO> {
                public:
                    static DigitalIOFactory instance;

                    DigitalIOFactory();

                    const std::string &getConsumerName() const noexcept;

                    void destroyChip(const std::string &name);

                private:
                    std::shared_ptr<spacepi::resource::DigitalIO> create(const std::string &name);

                    std::string consumerName;
                    std::unordered_map<std::string, std::weak_ptr<DigitalIOChip>> chips;
            };
        }
    }
}

#endif
