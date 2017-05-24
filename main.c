#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sptg_file.h"

float x_min = 150;
float x_max = 150;
float y_min = 150;
float y_max = 150;

void scrapeCords(char line[]);



int main(int argc, char const *argv[]) {
    FILE *inputFile = fopen(argv[1], "r+");
    char line[200];
    uint16_t lineBuffer_size = 200;
    char nl = 0;

    if (inputFile != NULL){
        while ( fgets(line, 200, inputFile) != NULL){
            // printf("%s", line);
            switch (line[0]){
                case 'G':
                    if (line[1] == '1') scrapeCords(line);
                    break;

                case 'T':       // Tool change
                    // moveToPosition();
                    // changeMaterial();
                    // drawTower();
                    break;

                case 'M':
                    //stop scraping codantes
                    //end homing moves will throw off tower placement

                case ';':       //Get info about user config
                    // scrapeConfig(line)
                    break;

                // default:

            } //switch
        } //while
    } //if
    else{
        perror("File failed to open\n");
    }
    printf("Xmin: %f    |    Xmax: %f\n", x_min, x_max );
    printf("Ymin: %f    |    Ymax: %f\n", y_min, y_max );
    return 0;
}

// tower based on the size of the melt zone of hot end?



void scrapeCords(char line[]){
    const char key[2] = " ";
    char *feild = strtok(line, key);
    float num = 0;
    char* temppter = NULL;

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
    
}


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