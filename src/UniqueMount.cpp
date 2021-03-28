#include <cerrno>
#include <cstring>
#include <exception>
#include <memory>
#include <SpacePi.hpp>
#include <spacepi/liblinux/UniqueMount.hpp>
#include <sstream>
#include <string>
#include <sys/mount.h>

using namespace std;
using namespace spacepi::liblinux;

UniqueMount::UniqueMount(const std::string &blockDevice, const std::string &mountPoint, const std::string &options, const std::string &type){
    this->blockDevice = blockDevice;
    this->mountPoint = mountPoint;
    this->mounted = false;
    flags = MS_RELATIME;
    flagSet(options);
    this->type = type;
    mount();
}

UniqueMount::~UniqueMount() noexcept(false) {
    unmount();
}

UniqueMount::UniqueMount(UniqueMount &&move) noexcept{
    this->blockDevice = move.blockDevice;
    this->mountPoint = move.mountPoint;
    this->flags = move.flags;
    this->options = move.options;
    this->type = move.type;
    this->mounted = move.mounted;

    move.mounted = false;
}

UniqueMount &UniqueMount::operator =(UniqueMount &&move) noexcept{
    this->blockDevice = move.blockDevice;
    this->mountPoint = move.mountPoint;
    this->flags = move.flags;
    this->options = move.options;
    this->type = move.type;
    this->mounted = move.mounted;

    move.mounted = false;
    return *this;
}

bool UniqueMount::isMounted() const noexcept{
    return mounted;
}

void UniqueMount::mount(){
    try {
        if(!mounted){
            handle(::mount(blockDevice.c_str(),mountPoint.c_str(),type.c_str(),flags,options.c_str()))
                << "Failed to mount '" << blockDevice << "' to '" << mountPoint << "': " << SyscallErrorString;
            mounted = true;
            if ((flags & MS_BIND) != 0) {
                handle(::mount("none",mountPoint.c_str(),"none",flags | MS_REMOUNT,options.c_str()))
                    << "Failed to mount '" << blockDevice << "' to '" << mountPoint << "': " << SyscallErrorString;
            }
        }
    } catch (const exception &e) {
        if (!(flags & MS_SILENT)) {
            throw;
        }
    }
}

void UniqueMount::unmount(){
    if(mounted){
        sync();
        handle(umount(mountPoint.c_str()))
            << "Failed to unmount '" << mountPoint << "': " << SyscallErrorString;
        mounted = false;
    }
}

void UniqueMount::forceUnmount() {
    try {
        unmount();
    } catch (const std::exception &) {
        mounted = false;
        throw;
    }
}

void UniqueMount::remount(const std::string &options){
    flagSet(options);
    if (mounted) {
        handle(::mount("none",mountPoint.c_str(),"none",flags | MS_REMOUNT,this->options.c_str()))
            << "Failed to mount '" << blockDevice << "' to '" << mountPoint << "': " << SyscallErrorString;
    }
}

void UniqueMount::flagSet(const std::string &options){
    std::unique_ptr<char[]> buffer(new char[options.size()+1]);
    strcpy(buffer.get(), options.c_str());
    std::ostringstream optionstream;
    bool written = false;
    if (!this->options.empty()) {
        optionstream << this->options;
        written = true;
    }
    for(char *ptr = strtok(buffer.get(), ","); ptr != nullptr; ptr = strtok(nullptr, ",")){
        std::string option(ptr);
        if(option == "async"){
            flags = ~MS_SYNCHRONOUS & flags;
        }
        else if (option == "sync"){
            flags = flags | MS_SYNCHRONOUS;
        }
        else if (option == "atime"){
            flags = ~MS_NOATIME & flags;
        }
        else if(option == "noatime"){
            flags = flags | MS_NOATIME;
        }
        else if(option == "strictatime"){
            flags = flags | MS_STRICTATIME;
        }
        else if(option == "nostrictatime"){
            flags = ~MS_STRICTATIME & flags;
        }
        else if(option == "relatime"){
            flags = flags | MS_RELATIME;
        }
        else if(option == "norelatime"){
            flags = ~MS_RELATIME & flags;
        }
        else if(option == "auto"){

        }
        else if (option == "noauto"){

        }
        else if (option == "defaults") {
            flags = ~MS_RDONLY & flags;
            flags = ~MS_NOSUID & flags;
            flags = ~MS_NODEV & flags;
            flags = ~MS_SYNCHRONOUS & flags;
            flags = flags | MS_RELATIME;
        }
        else if (option == "dev") {
            flags = ~MS_NODEV & flags;
        }
        else if (option == "nodev"){
            flags = flags | MS_NODEV;
        }
        else if (option == "diratime"){
            flags = ~MS_NODIRATIME & flags;
        }
        else if (option == "nodiratime"){
            flags = MS_NODIRATIME | flags;
        }
        else if (option == "dirsync"){
            flags = MS_DIRSYNC | flags;
        }
        else if (option == "exec"){
            flags = ~MS_NOEXEC & flags;
        }
        else if (option == "noexec"){
            flags = flags | MS_NOEXEC;
        }
        else if (option == "group"){
            flags = MS_NOSUID | flags;
            flags = MS_NODEV | flags;
        }
        else if (option == "mand"){
            flags = MS_MANDLOCK | flags;
        }
        else if (option == "nomand"){
            flags = ~MS_MANDLOCK & flags;
        }
        else if (option == "nofail") {
            flags = MS_SILENT | flags;
        }
        else if (option == "suid"){
            flags = ~MS_NOSUID & flags;
        }
        else if (option == "nosuid"){
            flags = MS_NOSUID | flags;
        }
        else if (option == "owner"){
            flags = MS_NOSUID | flags;
            flags = flags | MS_NODEV;
        }
        else if (option == "remount"){
            flags = MS_REMOUNT | flags;
        }
        else if (option == "ro"){
            flags = MS_RDONLY | flags;
        }
        else if (option == "rw"){
            flags = ~MS_RDONLY & flags;
        }
        else if (option == "user"){
            flags = flags | MS_NOEXEC;
            flags = MS_NOSUID | flags;
            flags = flags | MS_NODEV;
        }
        else if (option == "nouser"){

        }
        else if (option == "users"){
            flags = flags | MS_NOEXEC;
            flags = MS_NOSUID | flags;
            flags = flags | MS_NODEV;
        }
        else if (option == "bind"){
            flags = MS_BIND | flags;
        }
        else {
            if(written) {
                optionstream << "," << option;
            } 
            else {
                optionstream << option;
                written = true;
            }
        }
    }
    this->options = optionstream.str();
}
