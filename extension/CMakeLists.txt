include(CheckCXXSourceCompiles)

spacepi_dependencies(
    GPIOD
    I2C
)

set(CMAKE_REQUIRED_LIBRARIES GPIOD)
check_cxx_source_compiles("
    #include <gpiod.hpp>

    int main() {
        gpiod::line l;
        return l.bias();
    }
" HAVE_LIBGPIOD_BIAS)

spacepi_extension(
    SOURCES
        src/detail/FilesystemFactoryBase.cpp
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
        src/SrcFilesystemFactory.cpp
        src/UART.cpp
        src/UARTFactory.cpp
        src/VarFilesystemFactory.cpp
    CONFIG_FILE include/spacepi/target/extension/Config.hpp
    INCLUDE include
    LINK
        GPIOD
        I2C
)
