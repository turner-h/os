#include "../kernel/fs.h"
#include "../lib/util.h"
#include "../lib/types.h"
#include "../drivers/ata.h"
#include "../drivers/vga.h"

BootRecord g_boot_record;

void init_fs() {
    u16 buffer[256];
    disk_read(0, BOOT_RECORD_ADDR, 1, buffer);
    memory_copy((u8 *)&buffer, (u8 *) &g_boot_record, sizeof(BootRecord));
    kprintf("sectors_per_cluster: %d\n", g_boot_record.sectors_per_cluster);
}

u32 sector_from_cluster(u32 cluster) {
    u32 first_data_sector = g_boot_record.reserved_sectors + (g_boot_record.number_of_fats * g_boot_record.sectors_per_fat);
    return first_data_sector + ((cluster - 2) * g_boot_record.sectors_per_cluster);
}

void read_cluster_chain(u32 addr) {
    u32 first_fat_sector = g_boot_record.reserved_sectors;
    
    u32 first_data_sector = g_boot_record.reserved_sectors + (g_boot_record.number_of_fats * g_boot_record.sectors_per_fat);
    u32 fat_offset = (addr - first_data_sector) >> 9;

    u32 fat_offset_sector = fat_offset / 512;
    u32 fat_offset_bytes = fat_offset % 512;

    u16 fat[256];
    disk_read(0, first_fat_sector + fat_offset_sector, 1, fat);

    u32 fat_entry = *((u32*) ((u8*) fat + fat_offset_bytes));     
}

int fs_open(char* file, u16* buffer) {
    u16 dir[256];
    disk_read(0, 96, 1, dir);

    int start = 1;
    int i = 1;  // skip the first '/'
    while (file[i] != 0) {
        if (file[i] == '/' || file[i] == '.') {
            u8 dir_entry = 0;
            u8 found_dir = 0;
            while (dir[(dir_entry * 0x10)] != 0 && !found_dir) {
                DirectoryEntry entry;
                memory_copy((u8*)&dir + (dir_entry * 0x20), (u8 *) &entry, sizeof(DirectoryEntry));

                int valid = string_cmp_case_sensitive(file + start, entry.name, i - start, 0);

                if (valid) {
                    u32 cluster = 0;
                    cluster += entry.first_cluster_high;
                    cluster <<= 8;
                    cluster += entry.first_cluster_low;
                    u32 addr = sector_from_cluster(cluster);

                    if (file[i] == '/') {
                        disk_read(0, (u32) addr, 1, dir);
                        memory_copy((u8*)&dir, (char *) &entry, sizeof(DirectoryEntry));

                        start = i + 1;
                        found_dir = 1;
                    } else {
                        if(string_cmp_case_sensitive(file + i + 1, entry.ext, 3, 0)) {
                            read_cluster_chain(addr);
                            disk_read(0, addr, 1, buffer);
                            return 0;
                        } else {
                            dir_entry++;
                            // extension doesnt match
                        }
                    }
                } else {
                    dir_entry++;

                    if (dir[(dir_entry * 0x10)] == 0) {
                        //cant find dir
                    }
                }
            }
        }

        i++;
    }

    return 1;
}
