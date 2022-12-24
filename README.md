# vfs
This is a fs simulator for studying vfs system.

## create a disk image
qemu-img
## fdisk
This tool is used to fdisk harddisk
## losetup(floppy) v.s. kpartx
losetup for floppy
kpartx for harddisk
kpartx -av ./img   // 
kpartx -d ./img

## mkfs 
mkfs -t ext2 /dev/loopxxx

## mount


##

dumpe2fs -h
## i-node
1. i-node idx start from 0, 

## block
1. block id start from 0, and block id is from beginning of the disk. not per group.
2. boot section locates in block 0, No. 0 sctor.
3. super block locates in No. 3 sector, either in block 0(2k, 4k size of block) or block1(1k size of block)
4. group description locates in the next block to super block.
