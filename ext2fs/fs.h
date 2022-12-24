#ifndef _FS_H_
#define _FS_H_
#include "ext2.h"
#include "type.h"

typedef struct tag_par{
    u8 boot_signature;
    u8 start_head;
    u8 start_sector;
    u8 start_cylinder;
    u8 system_signature;
    u8 end_head;
    u8 end_sector;
    u8 end_cylinder;
    u32 nrb_before;
    u32 nrb_inside;
} partition_t;

int init_hd(int dev_id);
void show_super(super_block_t *super);
int init_floppy();
void show_desp(group_desc_t *desp);

#endif //_FS_H_
