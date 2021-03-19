
// THIS CODE IS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING
// A TUTOR OR CODE WRITTEN BY OTHER STUDENTS - LAURA NEFF


#include <getopt.h> //for getopt
#include "inodemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct Options {
    char transformation;
    char *transformationValue;
    char *inputFile;
    char *outputFile;
} options;

int main( int argc, char *argv[] )
{
    int option;
    //char *endPointer;
    long target;

    options capture;
    // capture.transformation = 0;
    // capture.transformationValue = NULL;
    // capture.outputFile = NULL;

    while((option = getopt(argc, argv, "cxtf:]")) != -1) {
        switch(option) {
            case 'c':
            //capture = (options){.transformation = 'b', .transformationValue = optarg, .inputFile = "foo.pbm", .outputFile = "foo.ppm"};

            if(capture.transformation) //meaning if capture.transformation != 0
            {
                fprintf(stderr, "Error: Multiple transformations specified\n");
                exit(1);
            }

            capture.transformation = 'b';
            //capture.transformationValue = optarg;
            //printf("%s\n", capture.transformationValue);
            printf("%c\n", capture.transformation);

            //printf("Option b, converting to PBM.\n");
            break;

            case 'g':
            target = strtol(optarg, NULL, 10);

            if(capture.transformation) //meaning if capture.transformation != 0
            {
                fprintf(stderr, "Error: Multiple transformations specified\n");
                exit(1);
            }

      
            default:
                //For now
                fprintf(stderr, "Usage: ppmcvt [-bgirsmtno] [FILE]\n");
                exit(1);
                //printf("Unexpected option\n"); Take out comments later
            break;
        }
    }