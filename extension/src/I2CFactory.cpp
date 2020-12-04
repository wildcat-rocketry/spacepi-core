#include <memory>
#include <string>
#include <SpacePi.hpp>
#include <spacepi/target/extension/I2C.hpp>
#include <spacepi/target/extension/I2CFactory.hpp>

using namespace std;
using namespace spacepi;
using namespace spacepi::resource;
using namespace spacepi::util;
using namespace spacepi::target;
using namespace spacepi::target::extension;

I2CFactory I2CFactory::instance;

I2CFactory::I2CFactory() noexcept : ResourceFactory<resource::I2C>("dev") {
}

shared_ptr<resource::I2C> I2CFactory::create(const string &name) {
    size_t colon = name.find_first_of(':');
    if (colon == string::npos) {
        throw EXCEPTION(ResourceException("Invalid I2C resource name."));
    }
    string devName = name.substr(0, colon);
    string addr = name.substr(colon);
    return shared_ptr<resource::I2C>(new extension::I2C(devName, stoi(addr, nullptr, 16)));
}
