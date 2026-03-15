#ifndef ATA_H
#define ATA_H

#include "../lib/types.h"

int disk_read(int slave, u32 addr, u32 sectors, u16 *buffer);

#endif