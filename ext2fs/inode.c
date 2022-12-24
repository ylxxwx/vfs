#include "dir.h"
#include "inode.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "device.h"
#include "trace.h"

void show_inode(inode_t *inode) {
    if (inode == 0)
        return;
    trace("  ==========show inode=========%p\n", inode);
    trace("  mode:%x, uid:%x, size:%x, blocks:%d, flags:%x\n",
        inode->i_mode, inode->i_uid, inode->i_size, inode->i_blocks, inode->i_flags); 
    if (inode->i_mode & EXT2_S_IFREG)
        trace("   regular file\n");
    else if (inode->i_mode & EXT2_S_IFDIR)
        trace("   a dir\n");
    
    trace(    "blockId: %d-%d-%d\n", inode->i_block[0], inode->i_block[1], inode->i_block[2]);
    trace("  ==========show inode end=========%p\n", inode);

}

inode_t * read_inode(floppy_device_t *dev, int inode_id) {
    trace("============== read inode id:%d===========\n", inode_id);
    show_disk("read_inode", &dev->disk);
    if (dev == 0) {
        printf("read_inode, dev is nil.\n");
        return 0;
    }
    if (dev->ready == 0)
        init_dev(dev);

    inode_id -= 1;

    int block_id = dev->desp->bg_inode_table;
    int block_sz = dev->block_size;
    int sz_offset = inode_id * dev->super->s_inode_size;
    int block_offset = sz_offset / block_sz;
    block_id += block_offset;
    u32 inode_offset = inode_id % 4;

    //trace("==inode id:%d, block id:%d, offset:%d==\n", inode_id, block_id, inode_offset);
    //trace("inode sz: %d, block sz: %d, num in block: %d\n",
    //   dev->super->s_inode_size, block_sz, block_sz/dev->super->s_inode_size);
    //trace("block_id for inode : %d, inode offset: %d\n", block_id, inode_offset);
    u8 *buffer = 0;
    if (dev->inode_table.block_id != block_id) {
        if (dev->inode_table.buffer != 0)
            free(dev->inode_table.buffer);
        dev->inode_table.buffer = (u8*)malloc(block_sz);
        dev->inode_table.block_id = block_id;
        int nz = read_blocks(&dev->disk, block_id, dev->inode_table.buffer);
    }
    //trace("read block ret:%p\n", buffer);

    inode_t * inode = (inode_t *)dev->inode_table.buffer;
    inode = (inode_t *)((u8*)inode + 256 * inode_offset);
    //trace("buf:%p, inode:%p\n", buffer, inode);
    //show_inode(inode);

    /*
    if (inode->i_mode & EXT2_S_IFREG) {
        trace("...Regular file.\n");
        int data_block = inode->i_block[0];
        trace("...File block:%d\n", data_block);
        //if (dev->data_buf.block_id != data_block) {
        u8 *data_buffer = read_blocks(dev, data_block, 1);
        char *buf = malloc(inode->i_size + 1);
        memset(buf, 0, inode->i_size + 1);
        memcpy(buf, data_buffer, inode->i_size);
        trace("File Content: %s\n", buf);
        free(data_buffer);
        free(buf);
    }
    else if (inode->i_mode & EXT2_S_IFDIR) {
        int data_block = inode->i_block[0];
        trace("...Dir block:%d\n", data_block);
        //if (dev->data_buf.block_id != data_block) {
        u8 *data_buffer = read_blocks(dev, data_block, 1);
        //    dev->data_buf.block_id = data_block;
        //}
        dir_entry_t *dir = (dir_entry_t *)data_buffer;
        int size = 0;
        while(size < inode->i_size) {
            show_direntry(dir);
            size += dir->rec_len;
            dir = (dir_entry_t *)((u8*)dir + (u32)dir->rec_len);
        }
        dir = (dir_entry_t *)data_buffer;
        size = 0;
        while(size < inode->i_size) {
            //show_direntry(dir);
            if (dir->name[0] == '.') 
                trace("SPECIFIC FILE.\n");
            else if (strcmp(dir->name, "lost+found") == 0) 
                trace("lost+found.\n");
            else {
                trace("another node.\n");
                read_inode(dev, dir->inode);
            }
            size += dir->rec_len;
            dir = (dir_entry_t *)((u8*)dir + (u32)dir->rec_len);
        }
        free((void *)data_buffer); 
    }
    */
    //free((void *)buffer);
    trace("============== read inode end id:%d===========\n", inode_id);
    return inode;
}

u32 get_inode_mode(disk_t *disk, int inode_id) {
    show_disk("get_inode_mode: ", disk);
    floppy_device_t *device = get_device(disk);
    device->disk = *disk;
    inode_t *node =  read_inode(device, inode_id);
    if (node) 
        return node->i_mode;
    return INVALID_MODE;
}

int is_inode_file(disk_t *disk, int inode_id) {
    inode_t *node =  read_inode(get_device(disk), inode_id);
    if (node) 
        return (node->i_mode & EXT2_S_IFREG)?1:0;
    return 0;
}

int is_inode_dir(disk_t *disk, int inode_id) {
    inode_t *node =  read_inode(get_device(disk), inode_id);
    if (node) 
        return (node->i_mode & EXT2_S_IFDIR)?1:0;
    return 0;
}

void reset_dir_entry(disk_t *disk, int inode_id) {
    floppy_device_t *device = get_device(disk);
    inode_t *inode =  read_inode(device, inode_id);
    if (!inode) 
        return;
    int block_id = inode->i_block[0]; //i_block[0]
    if (device->dir_buffer.block_id != block_id) {
        if (device->dir_buffer.buffer)
            free(device->dir_buffer.buffer);
        device->dir_buffer.buffer = (u8*)malloc(device->block_size);
        device->dir_buffer.block_id = block_id;
    }
    device->dir_entry_idx = 0;//device->dir_buffer.buffer;
}

int get_next_dir_entry(disk_t *disk, int inode_id, char *name) {
    floppy_device_t *device = get_device(disk);
    inode_t *inode =  read_inode(device, inode_id);
    if (!inode) {
        printf("get_next_dir_entry read_inode failed.\n");
        return -1;
    }
    int block_id = inode->i_block[0]; //i_block[0]
    if (device->dir_buffer.block_id != block_id) {
        trace("get_next_dir_entry prepare the dir buffer.\n");
        if (device->dir_buffer.buffer)
            free(device->dir_buffer.buffer);
        device->dir_buffer.buffer = (u8*)malloc(device->block_size);
        int nz = read_blocks(&device->disk, block_id, device->dir_buffer.buffer);
        device->dir_buffer.block_id = block_id;
        device->dir_entry_idx = 0;//device->dir_buffer.buffer;
    }

    dir_entry_t *dir = (dir_entry_t *)((u8*)device->dir_buffer.buffer + device->dir_entry_idx);
    while (device->dir_entry_idx < inode->i_size) {
        //show_direntry(dir);
        if (dir->name[0] == '.')  {
            trace("SPECIFIC FILE.\n");
        }
        else if (strcmp(dir->name, "lost+found") == 0) {
            trace("lost+found.\n");
        }
        else {
            trace("found a file:%s\n", dir->name);
            strcpy(name, dir->name);
            device->dir_entry_idx += dir->rec_len;
            return dir->inode;
        }
        device->dir_entry_idx += dir->rec_len;
        dir = (dir_entry_t *)((u8*)dir + (u32)dir->rec_len);
    }
    return -3;
}

int get_inode_file(disk_t *disk, int inode_id, char *buf) {
    floppy_device_t *device = get_device(disk);
    inode_t *inode =  read_inode(device, inode_id);
    if (!inode) {
        printf("get_inode_file read_inode failed.\n");
        return -1;
    }
    int block_id = inode->i_block[0]; //i_block[0]
    if (device->data_buffer.block_id != block_id) {
        trace("get_next_dir_entry prepare the dir buffer.\n");
        if (device->data_buffer.buffer)
            free(device->data_buffer.buffer);
        device->data_buffer.buffer = (u8*)malloc(device->block_size);
        int nz = read_blocks(&device->disk, block_id, device->data_buffer.buffer);
        device->data_buffer.block_id = block_id;
    }
    memcpy(buf, device->data_buffer.buffer, inode->i_size);
    return inode->i_size;
}
