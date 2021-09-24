#ifndef SPACEPI_CORE_FORMAT_NATIVES_CLANG_HPP
#define SPACEPI_CORE_FORMAT_NATIVES_CLANG_HPP

#include <SpacePi/Format/Natives/Platform.hpp>

#ifdef __cplusplus
extern "C" {
#endif

SPACEPI_CORE_FORMAT_NATIVES_API
void SpacePi_Format_Natives_Clang_Construct();

SPACEPI_CORE_FORMAT_NATIVES_API
void *SpacePi_Format_Natives_Clang_LoadStyle(const char *formatFile);

SPACEPI_CORE_FORMAT_NATIVES_API
bool SpacePi_Format_Natives_Clang_Format(void *style, const char *codeFile, bool write);

SPACEPI_CORE_FORMAT_NATIVES_API
void SpacePi_Format_Natives_Clang_FreeStyle(void *style);

SPACEPI_CORE_FORMAT_NATIVES_API
void SpacePi_Format_Natives_Clang_Dispose();

#ifdef __cplusplus
}
#endif

#endif
