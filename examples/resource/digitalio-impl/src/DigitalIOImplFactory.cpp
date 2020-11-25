#include <memory>
#include <string>
#include <spacepi/example/DigitalIOImpl.hpp>
#include <spacepi/example/DigitalIOImplFactory.hpp>
#include <spacepi/resource/DigitalIO.hpp>
#include <spacepi/resource/ResourceFactory.hpp>

using namespace std;
using namespace spacepi::example;
using namespace spacepi::resource;

DigitalIOImplFactory DigitalIOImplFactory::instance;

DigitalIOImplFactory::DigitalIOImplFactory() : ResourceFactory<DigitalIO>("impl") {
}

shared_ptr<DigitalIO> DigitalIOImplFactory::create(const string &name) {
    return shared_ptr<DigitalIO>(new DigitalIOImpl());
}
