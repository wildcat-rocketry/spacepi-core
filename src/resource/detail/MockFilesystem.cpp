#include <iomanip>
#include <ios>
#include <memory>
#include <string>
#include <spacepi/log/AutoLog.hpp>
#include <spacepi/log/LogLevel.hpp>
#include <spacepi/log/LogStream.hpp>
#include <spacepi/resource/Filesystem.hpp>
#include <spacepi/resource/ResourceFactory.hpp>
#include <spacepi/resource/Stream.hpp>

namespace spacepi {
    namespace resource {
        namespace detail {
            class MockFile : public Stream, private spacepi::log::AutoLog<decltype("core:resource:mock:filesystem"_autolog)> {
                public:
                    MockFile(const std::string &name) noexcept;

                private:
                    int readBuf(char *buffer, int count);
                    int writeBuf(const char *buffer, int count);

                    std::string name;
            };

            class MockFilesystem : public Filesystem, private spacepi::log::AutoLog<decltype("core:resource:mock:filesystem"_autolog)> {
                public:
                    MockFilesystem(const std::string &name) noexcept;

                    std::shared_ptr<Stream> open(const std::string &name, enum OpenMode mode);

                private:
                    std::string name;
            };

            class MockFilesystemFactory : public ResourceFactory<Filesystem> {
                public:
                    static MockFilesystemFactory instance;

                    MockFilesystemFactory();

                private:
                    std::shared_ptr<Filesystem> create(const std::string &name);
            };
        }
    }
}

using namespace std;
using namespace spacepi::log;
using namespace spacepi::resource;
using namespace spacepi::resource::detail;

MockFilesystemFactory MockFilesystemFactory::instance;

MockFile::MockFile(const string &name) noexcept : name(name) {
}

int MockFile::readBuf(char *buffer, int count) {
    log(LogLevel::Debug) << "Reading file '" << name << "': " << count << " bytes";
    return count;
}

int MockFile::writeBuf(const char *buffer, int count) {
    LogStream os = log(LogLevel::Debug);
    os << "Writing file '" << name << "': " << count << "bytes:" << hex << setfill('0');
    for (int i = 0; i < count; ++i) {
        if (i % 8 == 0) {
            os << "\n   ";
        }
        os << " 0x" << setw(2) << (int) buffer[i] << setw(0);
    }
    return count;
}

MockFilesystem::MockFilesystem(const string &name) noexcept : name(name) {
    log(LogLevel::Debug) << "Created new filesystem '" << name << "'.";
}

shared_ptr<Stream> MockFilesystem::open(const string &name, enum OpenMode mode) {
    string combinedName = this->name + "'/'" + name;
    LogStream os = log(LogLevel::Debug);
    os << "Opening file '" << combinedName << "' in ";
    switch (mode) {
        case Read:
            os << "read";
            break;
        case Write:
            os << "write";
            break;
        case Log:
            os << "log";
            break;
        default:
            os << "unknown (" << mode << ")";
            break;
    }
    os << " mode";
    return shared_ptr<Stream>(new MockFile(combinedName));
}

MockFilesystemFactory::MockFilesystemFactory() : ResourceFactory<Filesystem>("mock") {
}

shared_ptr<Filesystem> MockFilesystemFactory::create(const string &name) {
    return shared_ptr<Filesystem>(new MockFilesystem(name));
}
