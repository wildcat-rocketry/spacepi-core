#include <memory>
#include <string>
#include <spacepi/resource/ADC.hpp>
#include <spacepi/resource/ResourceFactory.hpp>

using namespace std;
using namespace spacepi::resource;

shared_ptr<ADC> ADC::get(const string &name) {
    return ResourceFactory<ADC>::get(name);
}
