
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
#include <sys/time.h>
#include <sys/stat.h>
#include <errno.h>
#include <utime.h>


// #define MAPSIZE 1024
// char ** Map=NULL;


/*Step 1)
    - Go through the directory specified and find all files, directories, and links inside of it.
    - Understand relative paths, modes, modified times, inodes, etc.

    DONE

Step 2)
    - Fill appropriate structs with data from directories, files, and links scanned in step 1
    - Store everything in their appropriate linked lists

    DONE

Step 3) 
    - Write directories, files, and links to a tar file in the appropriate format 
    DONE

Step 4) 
    - Write a method that does steps 1-2, but starting from a tar file

    (Do for x --> opposite of what did for c -- start from tar file)
    (For t, just print details, don't extract)

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
    
          
        d = opendir(directory_name); //Directory pointer
        if(d == NULL){
            fprintf(stderr, "Error: Specified target (\"%s\") is not a directory.\n", directory_name);
            exit(-1);
        }

                    //Print size and name of each file in directory

                    //This is based on Dr. Arnold's classnotes

        fullname = (char *)malloc(sizeof(char)*(strlen(directory_name)+258));

        
        exists = lstat(directory_name, &buf); //Call stat on relative path and upload inode info into stat buf structure
        if(exists < 0){
            fprintf(stderr,"Error: Specified target (\"%s\") does not exist.\n", fullname);
            exit(-1);
            }      
        
         if(!get_inode( buf.st_ino )) {
             set_inode(buf.st_ino, directory_name);

            directory first_dir = malloc(dir_struct_size);
            first_dir->name = malloc(strlen(directory_name)*sizeof(char));
            strcpy(first_dir->name, directory_name);
            first_dir->name_length = strlen(directory_name); //This right?
            first_dir->mode = buf.st_mode;
            first_dir->modification_time = buf.st_mtime;
            first_dir->inode_number = buf.st_ino;
            
            insert_directory(first_dir);
         }


        for(de = readdir(d); de != NULL; de = readdir(d)){ //Read directory pointer and get directory entry
            sprintf(fullname, "%s/%s", directory_name, de->d_name);
            exists = lstat(fullname, &buf); //Call stat on relative path and upload inode info into stat buf structure
            if(exists < 0){
                fprintf(stderr,"Error: Specified target (\"%s\") does not exist.\n", fullname);
                exit(-1);
            }
            if(!get_inode( buf.st_ino )) {  //inode not yet seen; add to list and process            
                set_inode(buf.st_ino, fullname);
                if (S_ISDIR(buf.st_mode)) {
                    if (strcmp(de->d_name, ".") !=0 && strcmp(de->d_name, "..") !=0 ) { 
                        printf("%10lld %s/\n", (long long int) buf.st_size, fullname);
                        directory new_dir = malloc(dir_struct_size);
                        new_dir->name = malloc(strlen(fullname)*sizeof(char));
                        strcpy(new_dir->name, fullname);
                        new_dir->name_length = strlen(fullname); //This right?
                        new_dir->mode = buf.st_mode;
                        new_dir->modification_time = buf.st_mtime;
                        new_dir->inode_number = buf.st_ino;
                        insert_directory(new_dir);      
                        total_size += get_size(fullname);        
                    }  
                } else if (S_ISLNK(buf.st_mode) || buf.st_nlink > 1) {
                    printf("%10lld %s@\n", (long long int) buf.st_size, fullname);

                    hlink new_hlink = malloc(hlink_struct_size);
                    new_hlink->name = malloc(strlen(fullname)*sizeof(char));
                    strcpy(new_hlink->name, fullname);
                    new_hlink->name_length = strlen(fullname); //This right?
                    new_hlink->inode_number = buf.st_ino;
                    //new_file->content = fgetc((buf);
                    insert_hlink(new_hlink);
                    
                } else {
                    printf("%10lld %s\n", (long long int) buf.st_size, fullname);
                    file new_file = malloc(file_struct_size);
                    new_file->name = malloc(strlen(fullname)*sizeof(char));
                    strcpy(new_file->name, fullname);
                    new_file->name_length = strlen(fullname); //This right?
                    new_file->mode = buf.st_mode;
                    new_file->modification_time = buf.st_mtime;
                    new_file->inode_number = buf.st_ino;
                    new_file->size = buf.st_size;
                    //new_file->content = fgetc((buf);
                    insert_file(new_file);

                     //Access fields in buf to print out size and name in each file
                }
                total_size += buf.st_size;
        }
    }
        closedir(d);
        return total_size;


    }



    int tar_extract(const char *tarfile){

        struct stat buf;
        int exists = 0;
        //int off_t size, total_size = 0;
        DIR *d;
        //FILE *tar;
        size_t elements_read;

        struct dirent *de;
        char *fullname;


        FILE *tar = fopen(tarfile,"r"); //We are reading from the capture.archivefile
            if(tarfile == NULL){
                fprintf(stderr, "Error: Could not read file at this time. %s.\n", tarfile);
                exit(-1);
            } 
            
        
         uint32_t magic = 0;
         //elements_read = keeps track of how many times we called fread()
         elements_read = fread(&magic, 4, 1, tar); //Copies 4 bytes from the file into magic
         if(elements_read != 1) {
             fprintf(stderr, "Error: Reading error.\n");
            exit(-1);
         }

         if(magic != MAGIC){
             fprintf(stderr, "Error: Bad magic number (%d), should be: %d.\n", magic, MAGIC);
             exit(-1);
         }

         while(!feof(tar)){
              elements_read = 0; 
              u_int64_t inode_number;
              u_int32_t name_length; //Just so you know where the name ends in our tarfile format, because name is n bytes and is variable
              char *name;
              u_int32_t mode;
              u_int64_t modification_time;
              u_int64_t size;

              elements_read += fread(&inode_number, 8, 1, tar);
              elements_read += fread(&name_length, 4, 1, tar);
              name = malloc(name_length*sizeof(char)); //Rule: if we don't know how big a variable will be of if we need copies of it, malloc it!
              elements_read += fread(name, name_length, 1, tar);
              elements_read += fread(&mode, 4, 1, tar);
              elements_read += fread(&modification_time, 8, 1, tar);

               if(elements_read != 5) {
                   if (feof(tar)){return 0;}
                    fprintf(stderr, "Error: Reading error.\n");
                    exit(-1);
                }
                if (S_ISDIR(mode)) { //Mode is the only thing that tells you whether it's a file or a directory.
                     directory new_dir = malloc(dir_struct_size);
                     new_dir->name = name; //Generated upon file creation. Nothing else to say to hard disk.
                     new_dir->name_length = name_length; 
                     new_dir->mode = mode; //We have to tell hard disk the mode; it will assume the default of the current working directory plus the user who created the file (Linux stuff) otherwise
                     new_dir->modification_time = modification_time; //We have to tell hard disk the modification time; it will think the directory is new upon creation otherwise
                     new_dir->inode_number = inode_number; //Generated upon file creation. Hard disk knows this. 
                     insert_directory(new_dir);
                     int result = mkdir(name,mode);
                     if(result == -1){
                         fprintf(stderr, "Error: Could not make directory.\n");
                         exit(-1);
                     }

                     struct timeval * moddyTimeVal = malloc(24);
                     moddyTimeVal->tv_sec = modification_time;
                     moddyTimeVal->tv_usec = 0;

                     struct timeval * accessTimeVal = malloc(24);
                     result = gettimeofday((struct timeval * restrict) accessTimeVal, 0);

                     struct timeval timevalArray[2];
                     timevalArray[0] = *accessTimeVal;
                     timevalArray[1] = *moddyTimeVal;
            

                    result =  utimes(name, timevalArray);
                    if(result == -1){
                        fprintf(stderr, "Error: Could not set modification time for directory %s.\n", name);
                        exit(-1);
                    }

                                 
                } else {
                    file new_file = malloc(file_struct_size);
                    new_file->name = name;
                    new_file->name_length = name_length;
                    new_file->mode = mode; 
                    new_file->modification_time = modification_time;
                    new_file->inode_number = inode_number;
                    
                    elements_read = fread(&size, 8, 1, tar);
                    if(elements_read != 1) {
                        fprintf(stderr, "Error: Reading error.\n");
                        exit(-1);
                    }
                    new_file->size = size;
                    insert_file(new_file);

                    FILE *out_file = fopen(name, "w");
                    if(out_file == NULL) {
                        fprintf(stderr, "Error: Could not create file %s.\n", name);
                        exit(-1);
                    }
                    char o;
                    while(size > 0){ //Stop when we've gone through the entire file
                        o=fgetc(tar); //Gets one byte from the tar file. It's the data part of the file (the content)
                        fputc(o, out_file); //Writes one byte to the new output file
                        size--; //Decrement size by one (which is one byte) each time
                    }
                    fclose(out_file);

                    int result =  chmod(name, mode);
                     if(result == -1){
                         fprintf(stderr, "Error: Could not set permissions for file %s.\n", name);
                         exit(-1);
                     }

                    
                     struct timeval * moddyTimeVal = malloc(24);
                     moddyTimeVal->tv_sec = modification_time;
                     moddyTimeVal->tv_usec = 0;

                     struct timeval * accessTimeVal = malloc(24);
                     result = gettimeofday((struct timeval * restrict) accessTimeVal, 0);

                     struct timeval timevalArray[2];
                     timevalArray[0] = *accessTimeVal;
                     timevalArray[1] = *moddyTimeVal;

                     result =  utimes(name, timevalArray);




                    if(result == -1){
                        fprintf(stderr, "Error: Could not set modification time for file %s.\n", name);
                        exit(-1);
                    }


                }
                



         }


     return 0;


   

    


    }


    int tar_printing(const char *tarfile){

        struct stat buf;
        int exists = 0;
        //int off_t size, total_size = 0;
        DIR *d;
        //FILE *tar;
        size_t elements_read;

        struct dirent *de;
        char *fullname;


        FILE *tar = fopen(tarfile,"r"); //We are reading from the capture.archivefile
            if(tarfile == NULL){
                fprintf(stderr, "Error: Could not read file at this time. %s.\n", tarfile);
                exit(-1);
            } 
            
        
         uint32_t magic = 0;
         //elements_read = keeps track of how many times we called fread()
         elements_read = fread(&magic, 4, 1, tar); //Copies 4 bytes from the file into magic
         if(elements_read != 1) {
             fprintf(stderr, "Error: Reading error.\n");
            exit(-1);
         }

         if(magic != MAGIC){
             fprintf(stderr, "Error: Bad magic number (%d), should be: %d.\n", magic, MAGIC);
             exit(-1);
         }

         while(!feof(tar)){
              elements_read = 0; 
              u_int64_t inode_number;
              u_int32_t name_length; //Just so you know where the name ends in our tarfile format, because name is n bytes and is variable
              char *name;
              u_int32_t mode;
              u_int64_t modification_time;
              u_int64_t size;

              elements_read += fread(&inode_number, 8, 1, tar);
              elements_read += fread(&name_length, 4, 1, tar);
              name = malloc(name_length*sizeof(char)); //Rule: if we don't know how big a variable will be of if we need copies of it, malloc it!
              elements_read += fread(name, name_length, 1, tar);
              elements_read += fread(&mode, 4, 1, tar);
              elements_read += fread(&modification_time, 8, 1, tar);

               if(elements_read != 5) {
                   if (feof(tar)){return 0;}
                    fprintf(stderr, "Error: Reading error.\n");
                    exit(-1);
                }
                if (S_ISDIR(mode)) { //Mode is the only thing that tells you whether it's a file or a directory.
                     directory new_dir = malloc(dir_struct_size);
                     new_dir->name = name; //Generated upon file creation. Nothing else to say to hard disk.
                     new_dir->name_length = name_length; 
                     new_dir->mode = mode; //We have to tell hard disk the mode; it will assume the default of the current working directory plus the user who created the file (Linux stuff) otherwise
                     new_dir->modification_time = modification_time; //We have to tell hard disk the modification time; it will think the directory is new upon creation otherwise
                     new_dir->inode_number = inode_number; //Generated upon file creation. Hard disk knows this. 
                     insert_directory(new_dir);

                 printf("%s/ -- inode: %llu, mode: %o, mtime: %llu\n", name, (long long unsigned int) inode_number, mode, (long long unsigned int) modification_time);

                   
            

                                 
                } else {
                    file new_file = malloc(file_struct_size);
                    new_file->name = name;
                    new_file->name_length = name_length;
                    new_file->mode = mode; 
                    new_file->modification_time = modification_time;
                    new_file->inode_number = inode_number;
                    
                    elements_read = fread(&size, 8, 1, tar);
                    if(elements_read != 1) {
                        fprintf(stderr, "Error: Reading error.\n");
                        exit(-1);
                    }
                    new_file->size = size;
                    insert_file(new_file);

                    fseek(tar, size, SEEK_CUR);

                    if(mode & (S_IXUSR | S_IXGRP | S_IXOTH)){
                        printf("%s* -- inode: %llu, mode: %o, mtime: %llu, size: %llu\n", name, (long long unsigned int) inode_number, mode, (long long unsigned int) modification_time, (long long unsigned int) size);
                    } else {
                        printf("%s -- inode: %llu, mode: %o, mtime: %llu, size: %llu\n", name, (long long unsigned int) inode_number, mode, (long long unsigned int) modification_time, (long long unsigned int) size);
                    }


                    
                    




                    


                }
                



         }


     return 0;


   

    


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

            FILE *tarfile = fopen(capture.archivefile, "w"); //We are writing to the archive file
            if(tarfile == NULL){
                fprintf(stderr, "Could not create %s.\n", capture.archivefile);
                exit(-1);
            } 
            uint32_t magic = MAGIC;
            fwrite(&magic, 4, 1, tarfile);

    // u_int64_t inode_number;
    // u_int32_t name_length;
    // char *name;
    // u_int32_t mode;
    // u_int64_t modification_time;
    // struct Directory *next;



            directory temp = directory_head;
            while(temp != 0){
                fwrite(&temp->inode_number, 8, 1, tarfile);
                fwrite(&temp->name_length, 4, 1, tarfile);
                fwrite(temp->name, strlen(temp->name), 1, tarfile);
                fwrite(&temp->mode, 4, 1, tarfile);
                fwrite(&temp->modification_time, 8, 1, tarfile);
                temp = temp->next;
            }
            file tmpfile = file_head;
            char o;
            while(tmpfile !=0){
                fwrite(&tmpfile->inode_number, 8, 1, tarfile);
                fwrite(&tmpfile->name_length, 4, 1, tarfile);
                fwrite(tmpfile->name, strlen(tmpfile->name), 1, tarfile);
                fwrite(&tmpfile->mode, 4, 1, tarfile);
                fwrite(&tmpfile->modification_time, 8, 1, tarfile);
                fwrite(&tmpfile->size, 8, 1, tarfile);
                FILE *inptr = fopen(tmpfile->name,"r");
                if(inptr == 0){
                    fprintf(stderr,"Error: Couldn't open %s for reading.",tmpfile->name);
                }
                while((o=fgetc(inptr))!=EOF){
                    fputc(o,tarfile);
                }
                fclose(inptr);
                tmpfile = tmpfile->next;
            }

            hlink tempy = hlink_head;
            while(temp != 0){
                fwrite(&tempy->inode_number, 8, 1, tarfile);
                fwrite(&tempy->name_length, 4, 1, tarfile);
                fwrite(tempy->name, strlen(temp->name), 1, tarfile);
                tempy = tempy->next;
            }




            fclose(tarfile); //at the end
            break;
        case 'x':
            tar_extract(capture.archivefile);
            break;

        case 't':
            tar_printing(capture.archivefile);
            break;

     }



    }




