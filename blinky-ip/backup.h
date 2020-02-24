#ifndef BACKUP_H
#define BACKUP_H

#include <stdint.h>

int backup_init(void);
int backup_write(uint32_t ipaddr);

#endif
