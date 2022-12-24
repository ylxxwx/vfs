#include "disk.h"
#include "trace.h"

#define DIMENSION_DISK 2

typedef char* diskname_t;
char * disks[DIMENSION_DISK][DIMENSION_DISK] = {
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
    printf("ver:%d-%d\n", disk->major, disk->minor);
    return disks[disk->major][disk->minor];
}

void show_disk(char *prefix, disk_t *disk) {
    trace("%s  %d:%d:%d, name:%s\n", prefix, disk->major, disk->minor, disk->partition, disk_name(disk));
}
