#include "fs.h"
#include "type.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "driver.h"
#include "ext2.h"
#include "device.h"
#include "trace.h"

typedef struct {
    partition_t table;
    super_block_t super;
    group_desc_t *desp;
    u8* buffer[4];
} part_ctl_t;

typedef struct {
    part_ctl_t* part_ctrl[4];
} hd_ctl_t;

//hd_ctl_t hd_ctl[2];

void show_super(super_block_t *super) {
    if (super) {
        trace("=====Super Block=====\n");
        trace("volume name:%s, uuid:%x-%x-%x-%x\n", super->s_volume_name, super->s_uuid[0],
            super->s_uuid[1],super->s_uuid[2],super->s_uuid[3]);
        trace("inode count:%d, avail inode:%d\n", super->s_inodes_count, super->s_free_inodes_count);
        trace("block count:%d, avail block:%d\n", super->s_blocks_count, super->s_free_blocks_count);
        trace("log :%d, first data block:%d\n", super->s_log_block_size, super->s_first_data_block);
        trace("magic num:%x, inode size: %d\n", super->s_magic, super->s_inode_size);
        trace("s_first_ino:%d, \n", super->s_first_ino);
    }
}

void show_desp(group_desc_t *desp) {
    if (desp) {
        trace("Block bitmap block:%d, inode bitmapblock:%d\n", desp->bg_block_bitmap, desp->bg_inode_bitmap);
        trace("Inode table block:%d, userd dir count:%d\n", desp->bg_inode_table, desp->bg_used_dirs_count);
        trace("free blocks count:%d, free inodes count:%d\n", desp->bg_free_blocks_count, desp->bg_free_inodes_count);
    }
}

void show_ptable(hd_ctl_t *hd_ctl) {
    for (int idx = 0; idx < 4; idx++) {
        //trace("show partition:%d\n", idx);
        part_ctl_t *part_ctrl = hd_ctl->part_ctrl[idx];
        if (part_ctrl != 0) {
            partition_t *table = &part_ctrl->table;
            if (table) {
                trace("=====Partition table=====\n");
                trace("boot Sig:%x, sys Sig:%x\n", table->boot_signature, table->system_signature);
                trace("start head:%d, sector:%d, cylinder:%d\n", table->start_head, table->start_sector, table->start_cylinder);
                trace("end head:%d, sector:%d, cylinder:%d\n", table->end_head, table->end_sector, table->end_cylinder);
                trace("sector before:%d, sector in %d\n", table->nrb_before, table->nrb_inside);
            }
            super_block_t *super = &part_ctrl->super;
            show_super(super);
        }
    }
}
/*
int init_hd(int dev_id) {
    dev_id %= 2;

    partition_t *ppt;

    u8 buffer[512];
    int sz = read_sector(dev_id, 0, 1, buffer);
    ppt = (partition_t *)(buffer + 0x1BE);

    for (int idx = 0; idx < 4; idx++) {
        if (ppt->boot_signature != 0x00 && ppt->boot_signature != 0x80) {
            printf("hd partition table is not correct.\n");
            break;
        }
        if (ppt->system_signature != 0x83) {
            printf("hd partition table is not Linux. idx:%d, %x\n", idx, ppt->system_signature);
            break;
        }
        part_ctl_t *part_ctl = (part_ctl_t *)malloc(sizeof(part_ctl_t));
        memcpy(&(part_ctl->table), ppt, sizeof(partition_t));
        sz = read_sector(dev_id, ppt->nrb_before + 2, 2, (u8*)&part_ctl->super);
        int block_size;
        if (sz == 2) {
            hd_ctl[dev_id].part_ctrl[idx] = part_ctl;
            block_size = 1024 << part_ctl->super.s_log_block_size;
            printf("block size:%d\n", block_size);
        } else {
            printf("Read super ctrl faied. %d\n", sz);
        }
        part_ctl->buffer[0] = malloc(block_size);
        sz = read_sector(dev_id, ppt->nrb_before + 2 + 2, 2, (u8*)part_ctl->buffer[0]);
        part_ctl->desp = (group_desc_t *)part_ctl->buffer[0];
        //show_desp(part_ctl->desp);
        ppt++;
    }
    show_ptable(&hd_ctl[dev_id]);
    return 0;
}
*/
const char *floppy = "floppy.img";

floppy_device_t fdisk;
