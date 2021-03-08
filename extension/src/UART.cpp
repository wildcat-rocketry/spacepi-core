#include <cerrno>
#include <chrono>
#include <streambuf>
#include <string>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <SpacePi.hpp>
#include <spacepi/target/extension/UART.hpp>

using namespace std;
using namespace std::chrono;
using namespace spacepi::util;
using namespace spacepi::concurrent;
using namespace spacepi::target::extension;

UART::UART(const string &deviceName) : fd(open(("/dev/" + deviceName).c_str(), O_RDWR)) {
    throwError(fd);
    struct termios tios;
    throwError(tcgetattr(fd, &tios));
    switch (cfgetospeed(&tios)) {
        case B0:
            set_baud(0);
            break;
        case B50:
            set_baud(50);
            break;
        case B75:
            set_baud(75);
            break;
        case B110:
            set_baud(110);
            break;
        case B134:
            set_baud(134);
            break;
        case B150:
            set_baud(150);
            break;
        case B200:
            set_baud(200);
            break;
        case B300:
            set_baud(300);
            break;
        case B600:
            set_baud(600);
            break;
        case B1200:
            set_baud(1200);
            break;
        case B1800:
            set_baud(1800);
            break;
        case B2400:
            set_baud(2400);
            break;
        case B4800:
            set_baud(4800);
            break;
        case B9600:
            set_baud(9600);
            break;
        case B19200:
            set_baud(19200);
            break;
        case B38400:
            set_baud(38400);
            break;
        case B57600:
            set_baud(57600);
            break;
        case B115200:
            set_baud(115200);
            break;
        case B230400:
            set_baud(230400);
            break;
        default:
            throw EXCEPTION(ResourceException("Unknown UART BAUD rate reported by device."));
    }
}

int UART::getBAUDRate() const noexcept {
    return baud;
}

void UART::setBAUDRate(int baud) {
    speed_t speed;
    switch (baud) {
        case 0:
            speed = B0;
            break;
        case 50:
            speed = B50;
            break;
        case 75:
            speed = B75;
            break;
        case 110:
            speed = B110;
            break;
        case 134:
            speed = B134;
            break;
        case 150:
            speed = B150;
            break;
        case 200:
            speed = B200;
            break;
        case 300:
            speed = B300;
            break;
        case 600:
            speed = B600;
            break;
        case 1200:
            speed = B1200;
            break;
        case 1800:
            speed = B1800;
            break;
        case 2400:
            speed = B2400;
            break;
        case 4800:
            speed = B4800;
            break;
        case 9600:
            speed = B9600;
            break;
        case 19200:
            speed = B19200;
            break;
        case 38400:
            speed = B38400;
            break;
        case 57600:
            speed = B57600;
            break;
        case 115200:
            speed = B115200;
            break;
        case 230400:
            speed = B230400;
            break;
        default:
            throw EXCEPTION(ArgumentException("Unknown UART BAUD rate."));
    }
    struct termios tios;
    throwError(tcgetattr(fd, &tios));
    cfsetspeed(&tios, speed);
    cfmakeraw(&tios);
    throwError(tcsetattr(fd, TCSANOW, &tios));
    set_baud(baud);
}

streamsize UART::xsgetn(char *buffer, streamsize count) {
    if(last != 0 && count > 0){
        buffer[0] = last;
        last = 0;
        buffer++;
        count--;
	if(count == 0) return 1;
    }

    ssize_t r = ::read(fd, buffer, count);

    while(r == 0){
        Sleep::duration(baud_period);
        r = ::read(fd, buffer, count);
    }

    throwError(r);
    return r;
}

streamsize UART::xsputn(char *buffer, streamsize count) {
    ssize_t r = ::write(fd, buffer, count);
    throwError(r);
    return r;
}

void UART::throwError(int returnCode) {
    if (returnCode < 0) {
        throw EXCEPTION(ResourceException("UART operation failed."));
    }
}

void UART::set_baud(int baud) {
    this->baud = baud;
    this->baud_period = seconds(1) / double(baud);
}