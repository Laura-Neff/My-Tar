

#include <getopt.h> //for getopt
#include "inodemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct File {
    u_int32_t inode_number;
    u_int16_t name_length;
    char *name;
    u_int16_t mode;
    u_int32_t modification_time;
    u_int32_t size;
    char *content;
    file *next;
} file;

typedef struct Directory {
    u_int32_t inode_number;
    u_int16_t name_length;
    char *name;
    u_int16_t mode;
    u_int32_t modification_time;
    directory *next;
} directory;

typedef struct Hard_Links {
    u_int32_t inode_number;
    u_int16_t name_length;
    char *name;
    u_int16_t mode;
    u_int32_t modification_time;
    hlink *next;
} hlink;

