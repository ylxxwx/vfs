#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "type.h"
#include "disk.h"

#define SECTOR_SIZE 512
#define SUPER_SIZE  (2*SECTOR_SIZE)

int read_sector(disk_t *disk, int sector_id, int num, u8 *buf);
int read_file(const char *name, int sector_id, int num, u8 *buf);

#endif // _DRIVER_H_
