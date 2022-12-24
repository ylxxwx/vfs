#include "driver.h"
#include "stdlib.h"
#include "stdio.h"
#include "type.h"
#include "disk.h"

int read_sector(disk_t *disk, int sector_id, int num, u8 *buf) {
    char *name = disk_name(disk);
    printf("file to read:%s\n", name);
    return read_file(name, sector_id, num, buf);
}

int read_file(const char *name, int sector_id, int num, u8 *buf) {
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
