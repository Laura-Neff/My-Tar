

// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LAURA NEFF

#include <getopt.h> //for getopt
#include "inodemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define file_struct_size 64
#define dir_struct_size 64
#define hlink_struct_size 64
#define node_struct_size 25

#define MAGIC 0x7261746D

typedef struct Node {
    char type_id; //"f" for file, "d" for directory, "h" for hard link
    void *current;
    struct Node *next;
} * node;

typedef struct File {
    u_int64_t inode_number;
    u_int32_t name_length;
    char *name;
    u_int32_t mode;
    u_int64_t modification_time;
    u_int64_t size;
} * file;

typedef struct Directory {
    u_int64_t inode_number;
    u_int32_t name_length;
    char *name;
    u_int32_t mode;
    u_int64_t modification_time;
} * directory;

typedef struct Hard_Links {
    u_int64_t inode_number;
    u_int32_t name_length;
    char *name;
} * hlink;

// typedef struct timeval {
//     long tv_sec;
//     long tv_usec;
// } timeval;



