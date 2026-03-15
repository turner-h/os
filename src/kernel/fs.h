#ifndef FS_H
#define FS_H

#include "../lib/types.h"

#define BOOT_RECORD_ADDR 6 

void init_fs();
int fs_open(char* file, u16* buffer);

#endif