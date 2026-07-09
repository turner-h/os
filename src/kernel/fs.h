#ifndef FS_H
#define FS_H

#include "../lib/types.h"

#define BOOT_RECORD_ADDR 6 

typedef struct {
    u8 jump_code[3];
    u8 identifier[8];
    u16 bytes_per_sector;
    u8 sectors_per_cluster;
    u16 reserved_sectors;
    u8 number_of_fats;
    u16 root_dir_entries;
    u16 total_sectors;
    u8 media_descriptor_type;
    u16 sectors_per_fat_deprecated;
    u16 sectors_per_track;
    u16 number_of_heads;
    u32 hidden_sectors;
    u32 large_sector_count;
    u32 sectors_per_fat;
    u16 flags;
    u16 fat_version;
    u32 root_dir_cluster;
    u16 fsinfo_sector;
    u16 backup_boot_record_sector;
    u8 reserved[12];
    u8 drive_number;
    u8 nt_flags;
    u8 signature;
    u32 serial;
    u8 volume_label[11];
    u8 system_identifier[8];
} __attribute__((packed)) BootRecord;

typedef struct {
    char name[8];
    char ext[3];
    u8 flags;
    u8 reserved;
    u8 hundreths; //creation time in hundreths of seconds
    u16 creation_time;
    u16 creation_date;
    u16 last_accessed;
    u16 first_cluster_high;
    u16 last_modified_time;
    u16 last_modified_date;
    u16 first_cluster_low;
    u32 size;
} __attribute__((packed)) DirectoryEntry;

void init_fs();
int fs_open(char* file, u16* buffer);

#endif