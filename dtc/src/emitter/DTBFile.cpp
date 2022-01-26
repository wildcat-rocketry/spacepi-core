#include <fstream>
#include <string>

#include <spacepi/dtc/emitter/DTBFile.hpp>

using namespace std;
using namespace spacepi::dtc::emitter;

DTBFile::DTBFile(const string &filename) noexcept
    : stream(filename, ios::binary)
{
}

void DTBFile::write(const std::string &data) noexcept {
    stream << data;
}

void DTBFile::write(const uint64_t data) noexcept {
    stream << (char) (data >> 56);
    stream << (char) (data >> 48);
    stream << (char) (data >> 40);
    stream << (char) (data >> 32);
    stream << (char) (data >> 24);
    stream << (char) (data >> 16);
    stream << (char) (data >> 8);
    stream << (char) (0xFF & data); 
}

void DTBFile::write(const uint32_t data) noexcept {
    stream << (char) (data >> 24);
    stream << (char) (data >> 16);
    stream << (char) (data >> 8);
    stream << (char) (0xFF & data); 
}

void DTBFile::write(const uint8_t data) noexcept {
    stream << (char)data;
}

void DTBFile::write(const std::vector<uint8_t> &data) noexcept {
    for (const auto &uint8_t : data) {
        write(uint8_t);
    }
}

void DTBFile::write(const std::vector<uint32_t> &data) noexcept {
    for (const auto &uint32_t : data) {
        write(uint32_t);
    }
}

void DTBFile::write(const std::vector<uint64_t> &data) noexcept {
    for (const auto &uint64_t : data) {
        write(uint64_t);
    }
}

long DTBFile::position() noexcept {
    return stream.tellp();
}

void DTBFile::seek(long pos) noexcept {
    stream.seekp(pos);
}

void DTBFile::close() noexcept {
    stream.close();
}


