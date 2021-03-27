#include <cstddef>
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

                    std::shared_ptr<Stream> open(const std::string &name, enum OpenMode mode);

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

shared_ptr<Stream> StdFilesystem::open(const string &name, enum OpenMode mode) {
    string path = this->path + name;
    switch (mode) {
        case Read:
            return shared_ptr<Stream>(new StdFile(path, (ios_base::openmode) (ios_base::in | ios_base::binary)));
        case Write:
            return shared_ptr<Stream>(new StdFile(path, (ios_base::openmode) (ios_base::in | ios_base::binary | ios_base::out | ios_base::trunc)));
        case Log: {
            ifstream test1(path);
            if (!test1) {
                test1.close();
                return shared_ptr<Stream>(new StdFile(path, (ios_base::openmode) (ios_base::in | ios_base::binary | ios_base::out | ios_base::trunc)));
            }
            test1.close();
            size_t dot = path.find_last_of('.');
            size_t slash = path.find_last_of('/');
            string prefix;
            string suffix;
            if (dot != string::npos && (dot > slash || slash == string::npos)) {
                prefix = path.substr(0, dot) + "_";
                suffix = path.substr(dot);
            } else {
                prefix = path + "_";
            }
            for (int i = 1; true; ++i) {
                path = prefix + to_string(i) + suffix;
                ifstream test2(path);
                if (!test2) {
                    test2.close();
                    return shared_ptr<Stream>(new StdFile(path, (ios_base::openmode) (ios_base::in | ios_base::binary | ios_base::out | ios_base::trunc)));
                }
            }
        } break;
    }
    return shared_ptr<Stream>();
}

StdFilesystemFactory::StdFilesystemFactory() : ResourceFactory<Filesystem>("std") {
}

shared_ptr<Filesystem> StdFilesystemFactory::create(const string &name) {
    return shared_ptr<Filesystem>(new StdFilesystem(name));
}
