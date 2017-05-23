#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sptg_file.h"


int main(int argc, char const *argv[]) {
    FILE *inputFile = fopen(argv[1], "r+");
    char line[200];
    uint16_t lineBuffer_size = 200;
    char nl = 0;

    if (inputFile != NULL){
        while ( fgets(line, 200, inputFile) != NULL){
            printf("%s", line);
            switch (line[0]){
                case 'G':
                    scrapeCords(line); //Dynamically adjust size of tower/maybe dynamically size skirt?
                    break;

                case 'TOOLCHANGE':
                    break;

                case '':

                    break;

                default:

            } //switch
        } //while
    } //if
    else{
        perror("File failed to open\n");
    }
    return 0;
}

