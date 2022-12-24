#include "stdio.h"
#include "stdlib.h"

#include "type.h"
#include "trace.h"
#include "disk.h"

#define DIMENSION_DISK 2

typedef char* diskname_t;
static char * disks[DIMENSION_DISK][DIMENSION_DISK] = {
    {
        "floppya.img",
        "floppyb.img",
    },
    {   
        "hda.img",
        "hab.img",
    },
};

char *disk_name(disk_t *disk) {
    return disks[disk->major][disk->minor];
}

void show_disk(char *prefix, disk_t *disk) {
    trace("%s  %d:%d:%d, name:%s\n", prefix, disk->major, disk->minor, disk->partition, disk_name(disk));
}

static int read_file(const char *name, int sector_id, int num, u8 *buf) {
    FILE* read_file = fopen(name, "rb");
    if (!read_file) {
        printf("fopen %s failed", name);
        return -1;
    }

    fseek(read_file, 0L, SEEK_END);
    int fsz = ftell(read_file);
    rewind(read_file);

    int read_pos = sector_id * SECTOR_SIZE;
    if (read_pos >= fsz) {
        fclose(read_file);
        return -2;
    }

    fseek(read_file, read_pos, 0L);

    int rsz = fread(buf, SECTOR_SIZE, num, read_file);

    fclose(read_file);
    return rsz;
}

int read_sector(disk_t *disk, int sector_id, int num, u8 *buf) {
    char *name = disk_name(disk);
    return read_file(name, sector_id, num, buf);
}
