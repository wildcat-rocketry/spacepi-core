#include <string>
#include <spacepi/target/extension/detail/FilesystemFactoryBase.hpp>
#include <spacepi/target/extension/Config.hpp>
#include <spacepi/target/extension/SrcFilesystemFactory.hpp>

using namespace std;
using namespace spacepi::target::extension;
using namespace spacepi::target::extension::detail;

SrcFilesystemFactory SrcFilesystemFactory::instance;

SrcFilesystemFactory::SrcFilesystemFactory() noexcept : FilesystemFactoryBase("src") {
}

string SrcFilesystemFactory::getPath(const string &relativePath) noexcept {
    return CMAKE_SOURCE_DIR "/" + relativePath;
}
