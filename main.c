#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "vfs.h"
#include "disk.h"
#include "trace.h"

int split(char dst[][80], char* str, const char* spl)
{
    int n = 0;
    char *result = NULL;
    result = strtok(str, spl);
    while( result != NULL )
    {
        strcpy(dst[n++], result);
        result = strtok(NULL, spl);
    }
    return n;
}

#define CMD_LS          1
#define CMD_CD          2
#define CMD_MORE        3
#define CMD_EXIT        4
#define CMD_HELP        5
#define CMD_TRACEON     6
#define CMD_TRACEOFF    7
#define CMD_INVALID     8

void help(int cmd) {
    switch(cmd) {
        case CMD_HELP:
            printf("help            -- show all commands.\n");
            break;
        case CMD_LS:
            printf("ls              -- show current directory.\n");
            break;
        case CMD_CD:
            printf("cd [dir]        -- enter the next directory.\n");
            break;
        case CMD_MORE:
            printf("more [file]     -- show file contents.\n");
            break;
        case CMD_EXIT:
            printf("exit            -- exit this program.\n");
            break;
        case CMD_TRACEON:
            printf("traceon         -- enable trace.\n");
            break;
        case CMD_TRACEOFF:
            printf("traceoff        -- disable trace.\n");
            break;
        default:
            help(CMD_HELP);
            help(CMD_LS);
            help(CMD_CD);
            help(CMD_MORE);
            help(CMD_EXIT);
            help(CMD_TRACEON);
            help(CMD_TRACEOFF);
            break;
    }
}

const int str_2_cmd(char *str) {
    int cmd = CMD_INVALID;

    if (0 == strncmp(str, "ls", 2)) {
        cmd = CMD_LS;
    } else if (0 == strncmp(str, "cd", 2)) {
        cmd = CMD_CD;
    } else if (0 == strncmp(str, "more", 4)) {
        cmd = CMD_MORE;
    } else if (0 == strncmp(str, "exit", 4)) {
        cmd = CMD_EXIT;
    } else if (0 == strncmp(str, "help", 4)) {
        cmd = CMD_HELP;
    } else if (0 == strncmp(str, "traceon", 7)) {
        cmd = CMD_TRACEON;
    } else if (0 == strncmp(str, "traceoff", 8)) {
        cmd = CMD_TRACEOFF;
    }
    return cmd;
}

int main() {
    disk_t disk;
    disk.major = DISK_TYPE_FLOPPY;
    disk.minor= PIMARY_DISK;
    disk.partition = 0;
    enable_trace();
    mount_root(&disk);

    char buf[1024];

    while(1) {
        printf("> ");
        memset(buf, 0, 1024);
        char* ret = fgets(buf, 1000, stdin);
        if (ret == 0) {
            break;
        }

        char str[4][80];
        memset(str, 0, 4*80);
        int num = split(str, buf, " ");
        if (num == 0)
            continue;
        
        if (num > 0)
            str[num - 1][strlen(str[num - 1])-1] = 0;
        

        int cmd = str_2_cmd(str[0]);

        if (str[num-1][0] == '?') {
            help(cmd);
            continue;
        }

        switch(cmd) {
            case CMD_LS: 
                ls_cur_dir();
                break;
            case CMD_CD: {
                if (num == 2) {
                    cd_dir(str[1]);
                } else {
                    printf("input num:%d, expect 2.\n", num);    
                }
                break;
            }
            case CMD_MORE: {
                if (num != 2) {
                    printf("input num:%d, expect 2.\n", num);
                    return 0;
                } else {
                    more_file(str[1]);
                }
                break;
            }
            case CMD_TRACEON: {
                enable_trace();
                break;
            }
            case CMD_TRACEOFF: {
                disable_trace();
                break;
            }
            case CMD_HELP: {
                help(CMD_INVALID);
                break;
            }
            case CMD_INVALID:
            case CMD_EXIT: {
                return 0;
            }
        }
    }    
    return 0;
}
