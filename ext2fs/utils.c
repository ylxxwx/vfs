#include "utils.h"
#include "stdio.h"
#include "stdlib.h"

// The following function return sector Nr. of each part .

/*block_sz is log size*/
u32 sectors_in_block(int block_sz) {
    return (1 << (block_sz + 1));
}

/*offset per sector*/
u32 bootloader_offset() {
    return 0;
}

/*offset per sector*/
u32 superblock_offset(int partition_offset) {
    return partition_offset + 2;
}

/*offset per sector*/
u32 block_offset(int partition_offset, int block_id, int log_block_sz) {
    return partition_offset + block_id * sectors_in_block(log_block_sz);
}

/*
u32 block_offset_in_group1(int group1_offset, int block_sz, int block_id) {
    return group1_offset + block_id * sectors_in_block(block_sz);
}

u32 block_offset_sector(int group1_offset, int block_sz, int block_id, int group_id, int blocks_in_group) {
    return (block_offset_in_group1(group1_offset, block_sz, block_id) 
          + sectors_in_block(block_sz) * blocks_in_group * (group_id - 1));
}
*/
