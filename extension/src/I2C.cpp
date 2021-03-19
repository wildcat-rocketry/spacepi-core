#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
extern "C" {
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
}
#include <SpacePi.hpp>
#include <spacepi/target/extension/I2C.hpp>

using namespace std;
using namespace spacepi::util;
using namespace spacepi::target::extension;

I2C::I2C(const string &deviceName, uint16_t address) : address(address), fd(open(("/dev/" + deviceName).c_str(), O_RDWR)), errorChecking(false) {
    throwError("opening device", fd);
    if (address < 0x80) {
        ioctl(fd, I2C_TENBIT, 1);
    }
    throwError("setting i2c address", ioctl(fd, I2C_SLAVE, address));
}

int I2C::getSpeed() const noexcept {
    return 100000;
}

void I2C::setSpeed(int speed) {
    throw EXCEPTION(ResourceException("Unable to set bus speed (not implemented)."));
}

bool I2C::isSMBusErrorCheckingEnabled() const noexcept {
    return errorChecking;
}

void I2C::enableSMBusErrorChecking() {
    throwError("enabling SMBus error checking", ioctl(fd, I2C_PEC, 1));
}

void I2C::disableSMBusErrorChecking() {
    throwError("disabling SMBus error checking", ioctl(fd, I2C_PEC, 0));
}

void I2C::quickWriteSMBus(bool val) {
    throwError("executing SMBus quick write", i2c_smbus_write_quick(fd, val ? 1 : 0));
}

uint8_t I2C::readByteSMBus() {
    int r = i2c_smbus_read_byte(fd);
    throwError("executing SMBus byte read", r);
    return (uint8_t) r;
}

void I2C::writeByteSMBus(uint8_t data) {
    throwError("executing SMBus byte write", i2c_smbus_write_byte(fd, data));
}

uint8_t I2C::readByteSMBus(uint8_t command) {
    int r = i2c_smbus_read_byte_data(fd, command);
    throwError("executing SMBus byte read command", r);
    return (uint8_t) r;
}

void I2C::writeByteSMBus(uint8_t command, uint8_t data) {
    throwError("executing SMBus byte write command", i2c_smbus_write_byte_data(fd, command, data));
}

uint16_t I2C::readWordSMBus(uint8_t command) {
    int r = i2c_smbus_read_word_data(fd, command);
    throwError("executing SMBus word read command", r);
    return (uint16_t) r;
}

void I2C::writeWordSMBus(uint8_t command, uint16_t data) {
    throwError("executing SMBus word write command", i2c_smbus_write_word_data(fd, command, data));
}

uint16_t I2C::processCallSMBus(uint8_t command, uint16_t data) {
    int r = i2c_smbus_process_call(fd, command, data);
    throwError("executing SMBus process call", r);
    return (uint16_t) r;
}

void I2C::readBlockSMBus(uint8_t command, uint8_t *data) {
    throwError("executing SMBus read block", i2c_smbus_read_block_data(fd, command, data));
}

void I2C::writeBlockSMBus(uint8_t command, uint8_t *data, uint8_t length) {
    throwError("executing SMBus write block", i2c_smbus_write_block_data(fd, command, length, data));
}

void I2C::doTransaction(const vector<pair<uint8_t *, int16_t>> &steps) {
    int i = 0;
    int readByteCount = 0;
    vector<uint8_t> write_msgs;
    vector<uint8_t> read_msgs;
    vector<tuple<uint8_t*, uint8_t *, int16_t>> read_locations; 

    for (vector<pair<uint8_t *, int16_t>>::const_iterator it = steps.begin(); it != steps.end(); ++it) {
        if (it->second < 0) {
            if(read_msgs.size() > 0){
                throw EXCEPTION(ResourceException("I2C failed: May only read at end of transaction"));
            }

            if(write_msgs.capacity() < write_msgs.size() - it->second){
                write_msgs.resize(write_msgs.size() - it->second);
            }

            for(i = 0; i < it->second; i++){
                write_msgs.push_back(*(uint8_t *)(it->first + i));
            }
        } else {
            if(read_msgs.capacity() < readByteCount + it->second){
                read_msgs.resize(readByteCount + it->second);
            }

            read_locations.push_back(tuple<uint8_t*, uint8_t *, int16_t>(&read_msgs[readByteCount], steps[i].first, steps[i].second));
            readByteCount += steps[i].second;
        }

        i++;
    }

    struct i2c_msg msgs[2];
    if(write_msgs.size() > 0){
        msgs[0].addr = address;
        msgs[0].buf = &write_msgs[0];
        msgs[0].flags = (address >= 0x80)? I2C_M_TEN: 0x00;
        msgs[0].len = write_msgs.size();
    }

    if(readByteCount > 0){
        msgs[1].addr = address;
        msgs[1].buf = &read_msgs[0];
        msgs[1].flags = ((address >= 0x80)? I2C_M_TEN: 0x00) | I2C_M_RD;
        msgs[1].len = readByteCount;
    }

    struct i2c_rdwr_ioctl_data data;
    if(write_msgs.size() > 0){
        data.msgs = &msgs[0];
        if(readByteCount > 0){
            data.nmsgs = 2;
        } else {
            data.nmsgs = 1;
        }
    } else {
        data.msgs = &msgs[1];
        data.nmsgs = 1;
    }

    throwError("executing I2C transaction", ioctl(fd, I2C_RDWR, &data));

    for (auto it = read_locations.begin(); it != read_locations.end(); ++it) {
        memcpy(std::get<1>(*it) , std::get<0>(*it), std::get<2>(*it));
    }
}

void I2C::throwError(const string &action, int returnCode) {
    if (returnCode < 0) {
        throw EXCEPTION(ResourceException("I2C failed " + action + ": " + strerror(errno)));
    }
}

