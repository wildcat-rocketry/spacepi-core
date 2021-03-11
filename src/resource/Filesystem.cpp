#include <memory>
#include <string>
#include <spacepi/resource/Filesystem.hpp>
#include <spacepi/resource/ResourceFactory.hpp>

using namespace std;
using namespace spacepi::resource;

shared_ptr<Filesystem> Filesystem::get(const string &name) {
    return ResourceFactory<Filesystem>::get(name);
}
