

#include <getopt.h> //for getopt
#include "inodemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define file_struct_size 64
#define dir_struct_size 64
#define hlink_struct_size 64

#define MAGIC 0x7261746D

typedef struct File {
    u_int64_t inode_number;
    u_int32_t name_length;
    char *name;
    u_int32_t mode;
    u_int64_t modification_time;
    u_int64_t size;
    char *content;
    struct File *next;
} * file;

typedef struct Directory {
    u_int64_t inode_number;
    u_int32_t name_length;
    char *name;
    u_int32_t mode;
    u_int64_t modification_time;
    struct Directory *next;
} * directory;

typedef struct Hard_Links {
    u_int64_t inode_number;
    u_int32_t name_length;
    char *name;
    struct Hard_Links *next;
} * hlink;

// typedef struct timeval {
//     long tv_sec;
//     long tv_usec;
// } timeval;



