#ifndef _DISK_H_
#define _DISK_H_

#include "type.h"

#define DISK_TYPE_FLOPPY   0
#define DISK_TYPE_HARDDISK 1

#define PIMARY_DISK 0
#define SECONDARY_DISK 1

#define PARTITION_1 0
#define PARTITION_2 1
#define PARTITION_3 2
#define PARTITION_4 3

#define SECTOR_SIZE 512
#define SUPER_SIZE  (2*SECTOR_SIZE)


typedef struct {
  u8 major;
  u8 minor;  
  u8 partition;
} disk_t;

void show_disk(char *prefix, disk_t *disk);
int read_sector(disk_t *disk, int sector_id, int num, u8 *buf);

#endif //_DISK_H_
