#include "../lib/types.h"

int disk_read(int slave, u16 max_size, u32 addr, u32 sectors, u16 *buffer);