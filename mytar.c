
// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LAURA NEFF


#include <getopt.h> //for getopt
#include "inodemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


typedef struct Options {
    char specified;
    char *archivefile;
    char isThereF;
} options;

int main( int argc, char *argv[] )
{
    int option;
    //char *endPointer;
    long target;

    options capture;

    while((option = getopt(argc, argv, "cxtf:]")) != -1) {
        switch(option) {
            case 'c':

            if(capture.specified) //meaning if capture.transformation != 0
            {
                fprintf(stderr, "Error: Multiple modes specified.\n");
                exit(-1);
            }

            if(strlen(capture.isThereF) == 0){
                fprintf(stderr, "Error: No tarfile specified.\n");
                exit(-1);
                }
            
            printf("Creates an archive of the given directory tree. A directory name must be specified.");

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

            if(strlen(capture.isThereF) == 0){
                fprintf(stderr, "Error: No tarfile specified.\n");
                exit(-1);
                }

            printf("Extracts the directory tree contained in the specified archive");

            capture.specified = 'x';
            printf("%c\n", capture.specified);

            break;

            case 't':

            if(capture.specified) //meaning if capture.transformation != 0
            {
                fprintf(stderr, "Error: Multiple modes specified.\n");
                exit(-1);
            }

            if(strlen(capture.isThereF) == 0){
                fprintf(stderr, "Error: No tarfile specified.\n");
                exit(-1);
                }

            printf("Prints the contents of the specified archive");

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

            if(strlen(target) == 0){
                fprintf(stderr, "Error: No tarfile specified.\n");
                exit(-1);
            }
            //printf("Option g, converting to a PGM. Arg = %s\n", optarg);
            break;

      
            default:
                //For now
                if(strlen(capture.isThereF) == 0){
                fprintf(stderr, "Error: No tarfile specified.\n");
                exit(-1);
                }
                fprintf(stderr, "Error: No mode specified.\n");
                exit(-1);
                //printf("Unexpected option\n"); Take out comments later
            break;
        }
        // If a library/system call fails, mytarcallsperror()with the name of the failedroutinethen exits.
    }