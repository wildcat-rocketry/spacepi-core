#include <string>
#include <spacepi/liblinux/SharedTempDir.hpp>
#include <spacepi/liblinux/ResourceTempDir.hpp>

using namespace std;
using namespace spacepi::liblinux;

ResourceTempDir::ResourceTempDir(const string &name) : SharedTempDir(name) {
}
