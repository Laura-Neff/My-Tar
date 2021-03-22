
// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LAURA NEFF


#include <getopt.h> //for getopt
#include "inodemap.h"
#include "inodemap.c"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "mytar.h"
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

// #define MAPSIZE 1024
// char ** Map=NULL;


/*Step 1)
    - Go through the directory specified and find all files, directories, and links inside of it.
    - Understand relative paths, modes, modified times, inodes, etc.

Step 2)
    - Fill appropriate structs with data from directories, files, and links scanned in step 1
    - Store everything in their appropriate linked lists

Step 3) 
    - Write directories, files, and links to a tar file in the appropriate format 

Step 4) 
    - Write a method that does steps 1-2, but starting from a tar file

Step 5)
    - For extracting from tar, write methods to write everything in the linked lists back to disk


*/



 directory directory_head = 0;
 file file_head = 0;
 hlink hlink_head = 0;


typedef struct Options {
    char specified;
    char *archivefile;
    char isThereF;
    char *directory;
} options;

void insert_directory(directory n){
    directory temp = directory_head; //flag to check last node
    n->next = 0; //clear out flink just in case
    if(directory_head == 0) {
        directory_head = n;
        return;
    }
    else {
        while(temp->next != 0){ //List traversal
            if(temp->inode_number == n->inode_number){
                return;
            }
            temp = temp->next;
        }
        temp->next = n;
        return;
        //find last node
        //make last node next new node
    }


}

void insert_file(file n){
    file temp = file_head; //flag to check last node
    n->next = 0; //clear out flink just in case
    if(file_head == 0) {
        file_head = n;
        return;
    }
    else {
        while(temp->next != 0){ //List traversal
            if(temp->inode_number == n->inode_number){
                return;
            }
            temp = temp->next;
        }
        temp->next = n;
        return;
        //find last node
        //make last node next new node
    }

}

void insert_hlink(hlink n){
    hlink temp = hlink_head; //flag to check last node
    n->next = 0; //clear out flink just in case
    if(hlink_head == 0) {
        hlink_head = n;
        return;
    }
    else {
        while(temp->next != 0){ //List traversal
            if(temp->inode_number == n->inode_number){
                return;
            }
            temp = temp->next;
        }
        temp->next = n;
        return;
        //find last node
        //make last node next new node
    }


}






// This method is heavily inspired by Dr. Arnold's classnotes. //
int get_size(const char *directory_name){

        struct stat buf;
        int exists = 0;
        int total_size = 0;
        DIR *d;
        struct dirent *de;
        char *fullname;

        directory filled_directory;
        file filled_file;
        hlink filled_hlink;
    
          
        d = opendir(directory_name); //Directory pointer
        if(d == NULL){
            fprintf(stderr, "Error: Specified target (\"%s\") is not a directory.\n", directory_name);
            exit(-1);
        }

                    //Print size and name of each file in directory

                    //This is based on Dr. Arnold's classnotes

        fullname = (char *)malloc(sizeof(char)*(strlen(directory_name)+258));


        for(de = readdir(d); de != NULL; de = readdir(d)){ //Read directory pointer and get directory entry
            sprintf(fullname, "%s/%s", directory_name, de->d_name);
            exists = stat(fullname, &buf); //Call stat on relative path and upload inode info into stat buf structure
            if(exists < 0){
                fprintf(stderr,"Error: Specified target (\"%s\") does not exist.\n", fullname);
                exit(-1);
            }
            if(!get_inode( buf.st_ino )) {  //inode not yet seen; add to list and process            
                set_inode(buf.st_ino, fullname);

                if (S_ISDIR(buf.st_mode)) {
                    directory new_dir = malloc(sizeof(directory));
                    new_dir->name = malloc(sizeof(fullname));
                    strcpy(new_dir->name, fullname);
                    new_dir->name_length = strlen(fullname); //This right?
                    new_dir->mode = buf.st_mode;
                    new_dir->modification_time = buf.st_mtime;
                    new_dir->inode_number = buf.st_ino;
                    insert_directory(new_dir);
                    printf("%10lld %s/\n", buf.st_size, fullname);
                    if (strcmp(de->d_name, ".") !=0 && strcmp(de->d_name, "..") !=0 ) {            
                        total_size += get_size(fullname);        
                    }  
                } else if (S_ISLNK(buf.st_mode)) {
                    hlink new_hlink = malloc(sizeof(hlink));
                    new_hlink->name = malloc(sizeof(fullname));
                    strcpy(new_hlink->name, fullname);
                    new_hlink->name_length = strlen(fullname); //This right?
                    new_hlink->mode = buf.st_mode;
                    new_hlink->modification_time = buf.st_mtime;
                    new_hlink->inode_number = buf.st_ino;
                    //new_file->content = fgetc((buf);
                    insert_hlink(new_hlink);



                    printf("%10lld %s@\n", buf.st_size, fullname);
                } else if (buf.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
                    printf("%10lld %s*\n", buf.st_size, fullname);
                } else {
                    file new_file = malloc(sizeof(file));
                    new_file->name = malloc(sizeof(fullname));
                    strcpy(new_file->name, fullname);
                    new_file->name_length = strlen(fullname); //This right?
                    new_file->mode = buf.st_mode;
                    new_file->modification_time = buf.st_mtime;
                    new_file->inode_number = buf.st_ino;
                    new_file->size = buf.st_size;
                    //new_file->content = fgetc((buf);
                    insert_file(new_file);

                    printf("%10lld %s\n", buf.st_size, fullname); //Access fields in buf to print out size and name in each file
                }
                    total_size += buf.st_size;
        }
    }
        closedir(d);
        return total_size;


    }









int main( int argc, char *argv[] )
{
    

    struct stat buf;
    int exists = 0;
    int total_size = 0;
    DIR *d;
    struct dirent *de;


    int option;
    //char *endPointer;
    char* target;

    options capture;
    capture.directory = "";
    capture.archivefile = "";
    capture.specified = 0;
    capture.isThereF = 0;


    while((option = getopt(argc, argv, "cxtf:]")) != -1) {
        switch(option) {
            case 'c':

                if(capture.specified) //meaning if capture.specified != 0
                {
                    fprintf(stderr, "Error: Multiple modes specified.\n");
                    exit(-1);
                }
                
                printf("Creates an archive of the given directory tree. A directory name must be specified.\n");

                capture.specified = 'c';
                printf("%c\n", capture.specified);

                //printf("Option b, converting to PBM.\n");
                break;

            case 'x':

                if(capture.specified) //meaning if capture.transformation != 0
                {
                    fprintf(stderr, "Error: Multiple modes specified.\n");
                    exit(-1);
                }


                printf("Extracts the directory tree contained in the specified archive\n");

                capture.specified = 'x';
                printf("%c\n", capture.specified);

                break;

            case 't':

                if(capture.specified) //meaning if capture.transformation != 0
                {
                    fprintf(stderr, "Error: Multiple modes specified.\n");
                    exit(-1);
                }

                printf("Prints the contents of the specified archive\n");

                capture.specified = 't';
                printf("%c\n", capture.specified);

                break;

            case 'f':
                target = optarg;

                // if(capture.specified) //meaning if capture.transformation != 0
                // {
                //     fprintf(stderr, "Error: Multiple modes specified\n");
                //     exit(-1);
                // }

                capture.isThereF = 'f';
                capture.archivefile = optarg;
                //printf("%s\n", capture.transformationValue);
                //printf("%c\n", capture.transformation);

                if(strlen(capture.archivefile) == 0){
                    fprintf(stderr, "Error: No tarfile specified.\n");
                    exit(-1);
                }
                //printf("Option g, converting to a PGM. Arg = %s\n", optarg);

                break;

      
            // default:
            //     //For now
            //     if(strlen(&capture.isThereF) == 0){
            //     fprintf(stderr, "Error: No tarfile specified.\n");
            //     exit(-1);
            //     }
            //     fprintf(stderr, "Error: No mode specified.\n");
            //     exit(-1);
            //     //printf("Unexpected option\n"); Take out comments later
            // break;
        }
        
    }

     if(argc != optind){
         capture.directory = argv[optind];

      }
      if ((strlen(capture.directory) == 0) && (capture.specified == 'c')) {
        fprintf(stderr, "Error: No directory target specified.\n");
        exit(-1);
     }

    if(strlen(&capture.isThereF) == 0){
        fprintf(stderr, "Error: No tarfile specified.\n");
        exit(-1);

    } else if (!capture.specified) {
        fprintf(stderr, "Error: No mode specified.\n");
        exit(-1);
    }

    switch(capture.specified) {
        case 'c':
            printf("%10d total\n", get_size(capture.directory));



     }



    }




