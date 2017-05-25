#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sptg_file.h"

float   x_min = 1000;
float   x_max = 0;
float   y_min = 1000;
float   y_max = 0;
uint8_t scrapeEN = 0;
uint8_t pastStart = 0;
uint8_t deferOutput = 0;
uint8_t emptyDeffer = 0;

void scrapeCords(char line[]);
void parseComment(char line[]);




int main(int argc, char const *argv[]) {
    FILE *inputFile = fopen(argv[1], "r");
    FILE *outputFile = fopen(".tmp", "w");
    char line[200];
    char templine[201];
    char deferbuff[100][200];
    uint8_t counter = 0;
    uint8_t temp = 0;
    uint16_t lineBuffer_size = 200;


    if (inputFile != NULL){
        while ( fgets(line, 200, inputFile) != NULL){
            
            switch (line[0]){
                case 'G':
                    if (line[1] == '1' && scrapeEN) scrapeCords(line);
                    break;

                case 'T':       // Tool change
                    // moveToPosition();
                    // changeMaterial();
                    // drawTower();
                    break;

                case 'M':
                    if ( (line[3] == '9') && (pastStart == 1) ){    
                        // remove line by putting a ; in front
                        strcpy(&templine[1], line);
                        templine[0] = ';';
                        strcpy(line, templine);
                    }
                    break;

                case ';':       //Get info about user config
                    parseComment(line);
                    break;

                // default:

            } //switch
            if (deferOutput){
                sprintf(deferbuff[counter], line);
                counter++;
            }
            else if ( emptyDeffer && (deferOutput == 0) ){
                for(temp; temp <= counter; temp++){
                    //printf("%d: %s\n", temp, deferbuff[temp]);
                    fputs(deferbuff[temp], outputFile);
                }

                emptyDeffer = 0;
                counter = 0;
                temp = 0;
                fputs(line, outputFile);
            }

            else fputs(line, outputFile); // Write any changes made to a line to output

        } //while
    } //if
    else{
        perror("File failed to open\n");
        return -1;
    }
    printf("Xmin: %f    |    Xmax: %f\n", x_min, x_max );
    printf("Ymin: %f    |    Ymax: %f\n", y_min, y_max );
    
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}

// tower based on the size of the melt zone of hot end?


void parseComment(char line[]){
    char linecpy[200];
    strcpy(linecpy, line);
    if ( (strcmp(linecpy, ";START\n")) == 0){
        pastStart = 1;
    }
    else if ( (strcmp(linecpy, ";STARTEND\n")) == 0){
        scrapeEN = 1;
    }
    else if ( (strcmp(linecpy, ";END\n")) == 0){
        scrapeEN = 0;
    }
    else if ( (strcmp(linecpy, ";EEND\n")) == 0){
        deferOutput = 1;
    }
    else if ( (strcmp(linecpy, ";ESTARTEND\n")) == 0){
        deferOutput = 0;
    }
    else if ( linecpy[1] == 'T') {
        emptyDeffer = 1;
    }
}



void scrapeCords(char line[]){
    const char key[2] = " ";
    float num = 0;
    char* temppter = NULL;
    char linecpy[200];
    strcpy(linecpy, line);

    char *feild = strtok(linecpy, key);

    while (feild != NULL){
        //printf("%s\n", feild);
        switch (feild[0]){
            case 'X':
                *feild++;
                num = strtof(feild, NULL);
                if (num > x_max) x_max = num;
                else if (num < x_min) x_min = num;
                break;
            case 'Y':
                *feild++;
                num = strtof(feild, NULL);
                if (num > y_max) y_max = num;
                else if (num < y_min) y_min = num;
                break;
            case 'Z':
                
                break;
        } //switch
        
        feild = strtok(NULL, key);
    } //while    
} // scrapeCords


//G1 X70.761 Y74.864 E0.01429
// G91                       ; Relative moves.
// G1 X50 Y50 Z2 F1000
// G1 E4 F500           ; Quicky extrude about the length of your melt zone.
// G1 E-2 F1000       ; Do a quick retraction a little shorter than your melt zone. This is to avoid stringing.
// G4 P2500             ; Wait 2.5 seconds so that filament can set.
// G1 E-108 F 8000  ; Quickly pull filament out of the Y-splitter.
// G90                       ; Abolute moves.

// ; Load in next extruder
// T0 ;  Select next material.
// G91                       ; Relative moves.
// G1 E105 F2000     ; Quickly load next material 95%.
// G1 E5 F2000         ; Slowly load last 5%.
// G1 Z-2 F500
// G90                       ; Abolute moves.
// T0
// G1 F5760
// G1 X70.851 Y74.786 E-0.04736