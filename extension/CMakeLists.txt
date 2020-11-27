include(CheckCXXSourceCompiles)

find_library(LibGPIOd NAMES libgpiod.so REQUIRED)
check_cxx_source_compiles("
    #include <gpiod.hpp>

    int main() {
        gpiod::line l;
        return l.bias();
    }
" HAVE_LIBGPIOD_BIAS)

spacepi_extension(
    src/detail/UniqueFD.cpp
    src/DigitalIO.cpp
    src/DigitalIOChip.cpp
    src/DigitalIOFactory.cpp
    src/I2C.cpp
    src/I2CFactory.cpp
)
if (HAVE_LIBGPIOD_BIAS)
    spacepi_extension_add_definitions(HAVE_LIBGPIOD_BIAS=1)
endif()
spacepi_extension_include_directories(PUBLIC
    include
)
spacepi_extension_link_libraries(PUBLIC
    ${LibGPIOd}
)
