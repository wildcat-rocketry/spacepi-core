#include <fstream>
#include <memory>
#include <string>
#include <spacepi/resource/Filesystem.hpp>
#include <spacepi/resource/ResourceFactory.hpp>
#include <spacepi/resource/Stream.hpp>
#include <spacepi/util/Exception.hpp>

namespace spacepi {
    namespace resource {
        namespace detail {
            class StdFile : public Stream {
                public:
                    StdFile(const std::string &name, std::ios_base::openmode mode);

                private:
                    int readBuf(char *buffer, int count);
                    int writeBuf(const char *buffer, int count);
                    int throwError(int ret) const;

                    std::fstream stream;
            };

            class StdFilesystem : public Filesystem {
                public:
                    StdFilesystem(const std::string &path) noexcept;

                    std::shared_ptr<Stream> open(const std::string &name, bool write);

                private:
                    std::string path;
            };

            class StdFilesystemFactory : public ResourceFactory<Filesystem> {
                public:
                    static StdFilesystemFactory instance;

                    StdFilesystemFactory();

                private:
                    std::shared_ptr<Filesystem> create(const std::string &name);
            };
        }
    }
}

using namespace std;
using namespace spacepi::resource;
using namespace spacepi::resource::detail;
using namespace spacepi::util;

StdFilesystemFactory StdFilesystemFactory::instance;

StdFile::StdFile(const string &name, ios_base::openmode mode) : stream(name, mode) {
    throwError(0);
}

int StdFile::readBuf(char *buffer, int count) {
    return throwError(stream.readsome(buffer, count));
}

int StdFile::writeBuf(const char *buffer, int count) {
    stream.write(buffer, count);
    stream.flush();
    return throwError(count);
}

int StdFile::throwError(int ret) const {
    switch (stream.rdstate()) {
        case eofbit:
            if (ret == 0) {
                return EOF;
            }
            break;
        case failbit:
            throw EXCEPTION(ResourceException("Logical error on I/O operation."));
        case badbit:
            throw EXCEPTION(ResourceException("Read/write error on I/O operation."));
    }
    return ret;
}

StdFilesystem::StdFilesystem(const string &path) noexcept : path(path) {
}

shared_ptr<Stream> StdFilesystem::open(const string &name, bool write) {
    return shared_ptr<Stream>(new StdFile(path + name, (ios_base::openmode) (ios_base::in | ios_base::binary | (write ? ios_base::out | ios_base::trunc : 0))));
}

StdFilesystemFactory::StdFilesystemFactory() : ResourceFactory<Filesystem>("std") {
}

shared_ptr<Filesystem> StdFilesystemFactory::create(const string &name) {
    return shared_ptr<Filesystem>(new StdFilesystem(name));
}
