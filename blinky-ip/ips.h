#ifndef IPS_H
#define IPS_H

#include <stdint.h>

int ips_reset(void);
int ips_next(uint32_t *addr);

#endif
