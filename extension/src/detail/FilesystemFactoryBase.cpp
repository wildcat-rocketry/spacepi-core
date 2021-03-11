#include <algorithm>
#include <memory>
#include <string>
#include <boost/filesystem.hpp>
#include <SpacePi.hpp>
#include <spacepi/target/extension/detail/FilesystemFactoryBase.hpp>

using namespace std;
using namespace boost::filesystem;
using namespace spacepi::resource;
using namespace spacepi::util;
using namespace spacepi::target::extension::detail;

string FilesystemFactoryBase::modulePath;

FilesystemFactoryBase::FilesystemFactoryBase(const string &name) noexcept : ResourceFactory<Filesystem>(name) {
}

shared_ptr<Filesystem> FilesystemFactoryBase::create(const string &name) {
    if (modulePath.empty()) {
        string fname = read_symlink("/proc/self/exe").filename().native();
        if (fname.substr(0, 12) != "spacepi-mod-") {
            throw EXCEPTION(ResourceException("Cannot use this resource outside of a SpacePi module."));
        }
        modulePath = fname.substr(12);
        replace(modulePath.begin(), modulePath.end(), '_', '/');
    }
    string p = getPath(modulePath + name);
    path parent;
    if (p.back() == '/') {
        parent = p;
    } else {
        parent = path(p).parent_path();
    }
    create_directories(parent);
    return Filesystem::get("std:" + p);
}
