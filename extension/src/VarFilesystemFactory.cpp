#include <string>
#include <spacepi/target/extension/detail/FilesystemFactoryBase.hpp>
#include <spacepi/target/extension/VarFilesystemFactory.hpp>

using namespace std;
using namespace spacepi::target::extension;
using namespace spacepi::target::extension::detail;

VarFilesystemFactory VarFilesystemFactory::instance;

VarFilesystemFactory::VarFilesystemFactory() noexcept : FilesystemFactoryBase("var") {
}

string VarFilesystemFactory::getPath(const string &relativePath) noexcept {
    return "/var/local/spacepi/" + relativePath;
}
