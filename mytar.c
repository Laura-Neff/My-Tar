
// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LAURA NEFF


#include <getopt.h> //for getopt
#include "inodemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "mytar.h"
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


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


// This has been inspired by Dr. Arnold's classnotes //



typedef struct Options {
    char specified;
    char *archivefile;
    char isThereF;
    char *directory;
} options;

int main( int argc, char *argv[] )
{
    
    
    directory *directories = 0;
    file *files = 0;
    hlink *hlinks = 0;

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

    int get_size(const char *directory_name){

        struct stat buf;
        int exists = 0;
        int total_size = 0;
        DIR *d;
        struct dirent *de;
        char *fullname;
    
          
        exists = stat(directory_name,&buf);
        if (exists < 0){
            fprintf(stderr, "Error: Specified target (\"%s\") does not exist.\n", directory_name);
            exit(-1);
        }
        d = opendir(directory_name); //Directory pointer
        if(d == NULL){
            fprintf(stderr, "Error: Specified target (\"%s\") is not a directory.\n", directory_name);
            exit(-1);
        }

                    //Print size and name of each file in directory

                    //This is based on Dr. Arnold's classnotes

        for(de = readdir(d); de != NULL; de = readdir(d)){ //Read directory pointer and get directory entry
            exists = lstat(de->d_name, &buf); //Call stat on directory name and upload inode info into stat buf structure
            if(exists < 0){
            fprintf(stderr,"Error: %s: %s\n",de->d_name, strerror(errno));
            exit(-1);
            } else if (S_ISDIR(buf.st_mode)) {
                printf("%10lld %s/\n", (long long int) buf.st_size, de->d_name);
            } else if (S_ISLNK(buf.st_mode)) {
                printf("%10lld %s@\n", (long long int) buf.st_size, de->d_name);
            } else if (buf.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
                printf("%10lld %s*\n", (long long int) buf.st_size, de->d_name);
            } else {
            printf("%10lld %s\n", (long long int) buf.st_size, de->d_name); //Access fields in buf to print out size and name in each file
            total_size += buf.st_size;
            }
        }
        //printf("%10d total\n", total_size);
        closedir(d);

        return total_size;







    }

