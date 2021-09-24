#ifndef SPACEPI_CORE_FORMAT_NATIVES_PLATFORM_HPP
#define SPACEPI_CORE_FORMAT_NATIVES_PLATFORM_HPP

#ifdef _WIN32
#ifdef __GNUC__
#define SPACEPI_CORE_FORMAT_NATIVES_API __attribute__((dllexport))
#else
#define SPACEPI_CORE_FORMAT_NATIVES_API __declspec(dllexport)
#endif
#else
#define SPACEPI_CORE_FORMAT_NATIVES_API
#endif

#endif
