#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "type.h"
#include "ext2.h"
#include "disk.h"

#define NUM_BLOCK_BUFFER 10
#define PATH_DEPTH       16
typedef struct {
    u8 *buffer;
    u32 block_id;
} block_buffer;

typedef char name_t[64] ;

typedef struct {
    disk_t disk;
    char *name;
    int ready;

    //int cur_dir;
    //int path_p;
    //name_t path[PATH_DEPTH];
    u32 partition_offset;
    super_block_t *super;
    group_desc_t  *desp;
    u32 block_size;
    u32           dir_entry_idx;
    block_buffer  dir_buffer;
    block_buffer  inode_table;
    block_buffer  data_buffer;
} floppy_device_t;

void init_dev(floppy_device_t *device);
int read_blocks(disk_t *disk, int start_block_id, u8* buffer);
floppy_device_t *get_device(disk_t *disk);
void read_desp(floppy_device_t *device);

#endif //_DEVICE_H_
