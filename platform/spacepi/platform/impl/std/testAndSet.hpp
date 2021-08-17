#include <spacepi/Platform.hpp>
#include <mutex>

namespace spacepi {
    namespace platform {
        namespace std {
            extern ::std::mutex testAndSetMutex;
        }
    }
}

#ifdef SPACEPI_PLATFORM_IMPL
std::mutex spacepi::platform::std::testAndSetMutex;
#endif

bool spacepi::Platform::testAndSet(volatile int &val) noexcept {
    std::unique_lock<std::mutex> lck(spacepi::platform::std::testAndSetMutex);
    int old = val;
    val = 1;
    return old != 0;
}

void spacepi::Platform::unset(volatile int &val) noexcept {
    std::unique_lock<std::mutex> lck(spacepi::platform::std::testAndSetMutex);
    val = 0;
}
