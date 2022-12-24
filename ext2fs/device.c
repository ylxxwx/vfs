#include "device.h"
#include "driver.h"
#include "stdio.h"
#include "stdlib.h"
#include "utils.h"
#include "inode.h"
#include "fs.h"
#include "disk.h"

floppy_device_t floppys[2][2];

floppy_device_t *get_device(disk_t *disk) {
    floppy_device_t * ret = &(floppys[disk->major][disk->minor]);
    return ret;
}

int get_harddisk_offset(disk_t *disk) {
    partition_t *ppt;

    u8 buffer[512];
    int sz = read_sector(disk, 0, 1, buffer);
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
        if (disk->partition == idx)
            return ppt->nrb_before;
        ppt++;
    }
    return 0;
}

int read_blocks(disk_t *disk, int start_block_id, u8* buffer) {
    floppy_device_t *device = get_device(disk);
    if (device->ready == 0) {
        device->disk = *disk;
        init_dev(device);
    }
    int sector_id = block_offset_in_group1(device->group1_offset, device->super->s_log_block_size, start_block_id);
    
    int sz = read_sector(disk, sector_id, sectors_in_block(device->super->s_log_block_size), (u8*)buffer);
    return sz;
}

void init_dev(floppy_device_t *device) {
    if (0 == device)
       return;
    trace("=============init_dev==========\n");
    disk_t *disk = &device->disk;
    show_disk("init_dev: ", disk);

    if(disk->major == 1) {
        device->group1_offset = get_harddisk_offset(disk);
        trace("partition offset:%d\n", device->group1_offset);
    } else {
        device->group1_offset = 0;
    }

    device->super = (super_block_t *)malloc(sizeof(super_block_t));
    int super_loc = superblock_offset_in_group1(device->group1_offset);
    int sz = read_sector(disk, super_loc, 2, (u8*)device->super);

    if (sz == 2) {
        show_super(device->super);
        device->ready = 1;
        int block_size = 512 * sectors_in_block(device->super->s_log_block_size);
        int log_block_size = device->super->s_log_block_size;
        device->block_size = block_size;
        read_desp(device);
    }
}

void read_desp(floppy_device_t *device) {
    if (device->super->s_magic == 0xef53) {
        trace("block id for super :%d\n", device->super->s_first_data_block);
        int block_id_for_desp = device->super->s_first_data_block +1;
        device->desp = (group_desc_t*)malloc(512*sectors_in_block(device->super->s_log_block_size));
        read_blocks(&device->disk, block_id_for_desp, (u8*)device->desp);
        show_desp(device->desp);
    }
}