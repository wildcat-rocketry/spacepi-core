#include <memory>
#include <string>
#include <spacepi/resource/ResourceFactory.hpp>
#include <spacepi/resource/UART.hpp>

using namespace std;
using namespace spacepi::resource;

shared_ptr<UART> UART::get(const string &name) {
    return ResourceFactory<UART>::get(name);
}
