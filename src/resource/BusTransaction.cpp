#include <array>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>
#include <spacepi/resource/BusTransaction.hpp>

using namespace std;
using namespace spacepi;
using namespace spacepi::resource;

template class BusBase<BusTransaction>;
template class BusBase<BusTransaction &>;

static array<uint8_t, 32> randomArray;

template <typename TReturn>
BusBase<TReturn>::~BusBase() noexcept(false) {
}

template <typename TReturn>
TReturn BusBase<TReturn>::operator <<(uint8_t data) noexcept {
    return write(&data, 1);
}

template <typename TReturn>
TReturn BusBase<TReturn>::seek(int count) noexcept {
    return seekSkip(&BusTransaction::write, count);
}

template <typename TReturn>
TReturn BusBase<TReturn>::operator >>(uint8_t &data) noexcept {
    return read(&data, 1);
}

template <typename TReturn>
TReturn BusBase<TReturn>::skip(int count) noexcept {
    return seekSkip(&BusTransaction::read, count);
}

template <typename TReturn>
template <typename Func>
TReturn BusBase<TReturn>::seekSkip(Func func, int count) {
    TReturn obj = ref();
    while (count > 0) {
        int num = count;
        if (num > randomArray.size()) {
            num = randomArray.size();
        }
        (obj.*func)(randomArray.data(), num);
        count -= num;
    }
    return obj;
}

BusTransaction Bus::read(uint8_t *data, uint16_t length) {
    return ref().read(data, length);
}

BusTransaction Bus::write(const uint8_t *data, uint16_t length) {
    return ref().write(data, length);
}

BusTransaction Bus::ref() noexcept {
    BusTransaction bus(*this);
    return bus;
}

BusTransaction::BusTransaction(Bus &bus) : bus(&bus), steps(new vector<pair<uint8_t *, int16_t>>()), alloc(new vector<vector<uint8_t>>()) {
}

BusTransaction::~BusTransaction() noexcept(false) {
    if (steps.unique()) {
        bus->doTransaction(*steps);
        steps->clear();
    }
}

BusTransaction &BusTransaction::read(uint8_t *data, uint16_t length) {
    steps->emplace_back(data, length);
    return *this;
}

BusTransaction &BusTransaction::write(const uint8_t *data, uint16_t length) {
    alloc->emplace_back();
    vector<uint8_t> &buf = alloc->back();
    buf.reserve(length);
    for (uint16_t i = 0; i < length; ++i) {
        buf.push_back(data[i]);
    }
    steps->emplace_back(buf.data(), -((int16_t) length));
    return *this;
}

BusTransaction &BusTransaction::ref() noexcept {
    return *this;
}

seek::seek(int count) noexcept : count(count) {
}

template <typename Target>
Target seek::operator ()(BusBase<Target> &os) const noexcept {
    return os.seek(count);
}

write::write(const uint8_t *data, uint16_t length) noexcept : data(data), length(length) {
}

template <typename Target>
Target write::operator ()(BusBase<Target> &os) const noexcept {
    return os.write(data, length);
}

skip::skip(int count) noexcept : count(count) {
}

template <typename Target>
Target skip::operator ()(BusBase<Target> &os) const noexcept {
    return os.skip(count);
}

read::read(uint8_t *data, uint16_t length) noexcept : data(data), length(length) {
}

template <typename Target>
Target read::operator ()(BusBase<Target> &os) const noexcept {
    return os.read(data, length);
}
