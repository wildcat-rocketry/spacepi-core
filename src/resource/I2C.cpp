#include <memory>
#include <string>
#include <spacepi/resource/I2C.hpp>
#include <spacepi/resource/ResourceFactory.hpp>

using namespace std;
using namespace spacepi::resource;

shared_ptr<I2C> I2C::get(const string &name) {
    return ResourceFactory<I2C>::get(name);
}
