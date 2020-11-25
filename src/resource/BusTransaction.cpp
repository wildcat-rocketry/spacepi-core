#include <cstdint>
#include <utility>
#include <vector>
#include <spacepi/resource/BusTransaction.hpp>
#include <spacepi/util/Exception.hpp>

using namespace std;
using namespace spacepi::resource;
using namespace spacepi::util;

BusTransaction Bus::operator <<(uint8_t &data) {
    BusTransaction trans(*this);
    trans << data;
    return trans;
}

BusTransaction Bus::operator <<(uint16_t &data) {
    BusTransaction trans(*this);
    trans << data;
    return trans;
}

BusTransaction Bus::operator <<(uint32_t &data) {
    BusTransaction trans(*this);
    trans << data;
    return trans;
}

BusTransaction Bus::operator >>(uint8_t &data) {
    BusTransaction trans(*this);
    trans >> data;
    return trans;
}

BusTransaction Bus::operator >>(uint16_t &data) {
    BusTransaction trans(*this);
    trans >> data;
    return trans;
}

BusTransaction Bus::operator >>(uint32_t &data) {
    BusTransaction trans(*this);
    trans >> data;
    return trans;
}

BusTransaction Bus::read(uint8_t *data, uint16_t length) {
    BusTransaction trans(*this);
    trans.read(data, length);
    return trans;
}

BusTransaction Bus::write(uint8_t *data, uint16_t length) {
    BusTransaction trans(*this);
    trans.write(data, length);
    return trans;
}

BusTransaction::BusTransaction(Bus &bus) : bus(bus) {
}

BusTransaction::~BusTransaction() {
    if (!steps.empty()) {
        bus.doTransaction(steps);
        steps.clear();
    }
}

BusTransaction::BusTransaction(BusTransaction &copy) : bus(copy.bus), steps(copy.steps) {
    copy.steps.clear();
}

BusTransaction &BusTransaction::operator =(BusTransaction &copy) {
    if (&bus != &copy.bus) {
        throw EXCEPTION(ResourceException("Cannot copy to transaction on different bus"));
    }
    steps.reserve(copy.steps.size());
    for (vector<pair<uint8_t *, int16_t>>::const_iterator it = copy.steps.begin(); it != copy.steps.end(); ++it) {
        steps.push_back(*it);
    }
    copy.steps.clear();
    return *this;
}

BusTransaction &BusTransaction::operator <<(uint8_t &data) {
    steps.emplace_back(&data, -1);
    return *this;
}

BusTransaction &BusTransaction::operator <<(uint16_t &data) {
    steps.emplace_back((uint8_t *) &data, -2);
    return *this;
}

BusTransaction &BusTransaction::operator <<(uint32_t &data) {
    steps.emplace_back((uint8_t *) &data, -4);
    return *this;
}

BusTransaction &BusTransaction::operator >>(uint8_t &data) {
    steps.emplace_back(&data, 1);
    return *this;
}

BusTransaction &BusTransaction::operator >>(uint16_t &data) {
    steps.emplace_back((uint8_t *) &data, 2);
    return *this;
}

BusTransaction &BusTransaction::operator >>(uint32_t &data) {
    steps.emplace_back((uint8_t *) &data, 4);
    return *this;
}

BusTransaction &BusTransaction::read(uint8_t *data, uint16_t length) {
    steps.emplace_back(data, length);
    return *this;
}

BusTransaction &BusTransaction::write(uint8_t *data, uint16_t length) {
    steps.emplace_back(data, -((int16_t) length));
    return *this;
}
