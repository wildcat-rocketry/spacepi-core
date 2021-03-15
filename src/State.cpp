#include <cstdint>
#include <string>
#include <SpacePi.hpp>
#include <spacepi/liblinux/State.hpp>
#include <spacepi/liblinux/StateData.pb.h>

using namespace std;
using namespace spacepi::util;
using namespace spacepi::liblinux;

State::State(const string &ser) {
    if (ser.size() % 2 != 0) {
        throw EXCEPTION(ArgumentException("Invalid serialized state string"));
    }
    string bin;
    bin.reserve(ser.size() / 2);
    for (string::const_iterator it = ser.begin(); it != ser.end(); ) {
        uint8_t data = 0;
        for (int i = 0; i < 2; ++i) {
            data <<= 4;
            if (*it >= '0' && *it <= '9') {
                data |= (*it - '0');
            } else if (*it >= 'A' && *it <= 'F') {
                data |= (*it - 'A' + 0xA);
            } else {
                throw EXCEPTION(ArgumentException("Invalid character in serialized state string"));
            }
            ++it;
        }
        bin.append(1, (char) data);
    }
    if (!data.ParseFromString(bin)) {
        throw EXCEPTION(ArgumentException("Invalid serialized state string format"));
    }
}

State::operator string() const {
    string bin;
    data.SerializeToString(&bin);
    string ser;
    ser.reserve(bin.size() * 2);
    for (string::const_iterator it = bin.begin(); it != bin.end(); ++it) {
        uint8_t data = (uint8_t) *it;
        for (int i = 0; i < 2; ++i) {
            uint8_t c = ((data & 0xF0) >> 4);
            if (c < 0xA) {
                ser.append(1, '0' + c);
            } else {
                ser.append(1, 'A' + c - 0xA);
            }
            data <<= 4;
        }
    }
    return ser;
}

StateData *State::operator ->() noexcept {
    return &data;
}

const StateData *State::operator ->() const noexcept {
    return &data;
}
