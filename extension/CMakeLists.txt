include(CheckCXXSourceCompiles)

find_library(LibGPIOd NAMES libgpiodcxx.so REQUIRED)
check_cxx_source_compiles("
    #include <gpiod.hpp>

    int main() {
        gpiod::line l;
        return l.bias();
    }
" HAVE_LIBGPIOD_BIAS)

find_library(LibI2C NAMES libi2c.so REQUIRED)

spacepi_extension(
    src/detail/UniqueDir.cpp
    src/detail/UniqueFD.cpp
    src/DigitalIO.cpp
    src/DigitalIOChip.cpp
    src/DigitalIOFactory.cpp
    src/I2C.cpp
    src/I2CFactory.cpp
    src/Memory.cpp
    src/MemoryFactory.cpp
    src/Processor.cpp
    src/ProcessorFactory.cpp
    src/SPI.cpp
    src/SPIFactory.cpp
    src/UART.cpp
    src/UARTFactory.cpp
)
if (HAVE_LIBGPIOD_BIAS)
    spacepi_extension_add_definitions(HAVE_LIBGPIOD_BIAS=1)
endif()
spacepi_extension_include_directories(PUBLIC
    include
)
spacepi_extension_link_libraries(PUBLIC
    ${LibGPIOd}
    ${LibI2C}
)
