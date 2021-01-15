#include <memory>
#include <string>
#include <SpacePi.hpp>
#include <spacepi/target/extension/UART.hpp>
#include <spacepi/target/extension/UARTFactory.hpp>

using namespace std;
using namespace spacepi;
using namespace spacepi::resource;
using namespace spacepi::target;
using namespace spacepi::target::extension;

UARTFactory UARTFactory::instance;

UARTFactory::UARTFactory() noexcept : ResourceFactory<resource::UART>("dev") {
}

shared_ptr<resource::UART> UARTFactory::create(const string &name) {
    return shared_ptr<resource::UART>(new extension::UART(name));
}
