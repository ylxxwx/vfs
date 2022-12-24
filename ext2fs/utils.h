#ifndef _UTILS_H_
#define _UTILS_H_

#include "type.h"

/*
block start from 0.
boot sector locates in block 0, sector 0.
super (two sectors) locates in block 0/1(depends), sector 3(fixed)
desp locates in the next block to super
*/

u32 sectors_in_block(int block_sz);
u32 bootblock_offset_in_group1(int group1_offset);
u32 firstblock_offset_in_group1(int group1_offset);
u32 superblock_offset_in_group1(int group1_offset);
/* 1. is super block.*/
u32 block_offset_in_group1(int group1_offset, int block_sz, int block_id);
/*block_sz is log size. 0: 1024(2 sector), 1: 2048(4 sector)*/
u32 firstdespblock_offset_in_group1(int group1_offset, int block_sz);
u32 block_offset_sector(int group1_offset, int block_sz, int block_id, int group_id, int blocks_in_group);

#endif // _UTILS_H_
