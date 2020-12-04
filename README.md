# SpacePi Target Library for Linux

## Building

### Building for Debugging

The utilities which make up the target can be built on any Linux system to facilitate debugging without needing a board to build and test on.

#### Requirements

The following are required to build the SpacePi Target Library for Linux for debugging:

* CMake version 3.11 or later (3.16 or later recommended)
* Make
* C++ Compiler toolchain supporting C++14
* ProtoBuf compiler supporting Proto3
* Boost libraries and headers
* LibGPIOd
* LibI2C

Most of these can be installed with the following command on Debian and similar:

```
# apt update
# apt install cmake build-essential protobuf-compiler libboost-all-dev libi2c-dev
```

The following can be used to install LibGPIOd:

```
$ git clone https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git
$ cd libgpiod
$ uname -r
$ git checkout v1.6 # Kernel version >= 5.5
$ git checkout v1.4 # Kernel version >= 4.8
$ mkdir build
$ cd build
$ ../autogen.sh --enable-bindings-cxx
$ make
# make install
```
