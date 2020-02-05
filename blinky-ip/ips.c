#include <stdint.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <spacepi.h>
#include "ips.h"

struct ifaddrs *ifa_alloc;
struct ifaddrs *ifa;

int is_ethernet(const char *ifname);

int ips_reset(void) {
    CHECK_ERROR(getifaddrs, &ifa_alloc);
    ifa = ifa_alloc;
    return 0;
}

int ips_next(uint32_t *addr) {
    while (ifa) {
        struct ifaddrs *iface = ifa;
        ifa = iface->ifa_next;
        if (is_ethernet(iface->ifa_name)) {
            if (iface->ifa_addr->sa_family == AF_INET) {
                struct sockaddr_in sa = *(struct sockaddr_in *) iface->ifa_addr;
                uint32_t a = sa.sin_addr.s_addr;
                *addr = (a & 0xFF000000) >> 24 |
                        (a & 0x00FF0000) >>  8 |
                        (a & 0x0000FF00) <<  8 |
                        (a & 0x000000FF) << 24;
                return TRUE;
            }
        }
    }
    if (ifa_alloc) {
        freeifaddrs(ifa_alloc);
        ifa_alloc = NULL;
    }
    return FALSE;
}

int is_ethernet(const char *ifname) {
    return *ifname == 'e';
}
