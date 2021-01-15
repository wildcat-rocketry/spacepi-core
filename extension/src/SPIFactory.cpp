#include <memory>
#include <string>
#include <SpacePi.hpp>
#include <spacepi/target/extension/SPI.hpp>
#include <spacepi/target/extension/SPIFactory.hpp>

using namespace std;
using namespace spacepi;
using namespace spacepi::resource;
using namespace spacepi::target;
using namespace spacepi::target::extension;

SPIFactory SPIFactory::instance;

SPIFactory::SPIFactory() noexcept : ResourceFactory<resource::SPI>("dev") {
}

shared_ptr<resource::SPI> SPIFactory::create(const string &name) {
    return shared_ptr<resource::SPI>(new extension::SPI(name));
}
