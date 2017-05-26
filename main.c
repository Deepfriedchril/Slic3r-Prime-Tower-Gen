#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINELENGTH 200

/******************************** Notes ****************************************
 *  Only need to do full prime tower layer after a material switch.
 *    Just do outline or infill(more work).
 * 
 * Sizing prime tower:
 *  ask user for area (l/w)?
 *  pull nozzle size from gcode
 *  pull feed rate from gcode?
 * 
 ******************************************************************************/


/**
 * Easy way to keep track of relative position.
 */
typedef enum Position{
    START       = 1,
    NORMAL      = 2,
    MATBEGIN    = 3,
    MATEND      = 4,
    FLUSH       = 5,
    END         = 6
} position;

typedef enum bool{
    false = 0,
    true  = 1
} bool;

/**
 * Singingly linked list
 */
typedef struct node {
    struct node *next;
    char   data[200];
} node_t;

void scrapecords(char *line);
void parsecomment(char *line);
void add_tail(node_t *head, char *val);
void flushlist(FILE *outputFile, node_t *head);


int     pos   = 0;       /*<Used to keep track of where we are in the gcode.*/
float   x_min = 1000;
float   x_max = 0;
float   y_min = 1000;
float   y_max = 0;


void main(int argc, char const *argv[]){
    if (argc < 1) exit(1);     // Make sure argument is supplied

    FILE *inputFile, *outputFile;               /*<I/O file pointers.*/
    char line[LINELENGTH];                      /*<Line buffer.*/
   
    node_t head;
    head.next = NULL;
    strcpy(head.data, "\0");

    inputFile  = fopen(argv[1], "r");
    if (inputFile == NULL) exit(1);
    outputFile = fopen(".tmp", "w");

    while( fgets(line, LINELENGTH, inputFile) != NULL ){
        switch (line[0]){
            case ';':           // Comment
                parsecomment(line);
                break;
            case 'G':           // Movement
                if (pos == NORMAL) scrapecords(line);  // Used for auto placement
                break;
            case 'M':           // Remove redundant waits for temp M109 by putting ';'
                if ( (pos >= START) && (line[3] == '9') ){
                    line[0] = ';';
                }

                break;

        } //switch

        if      (pos == MATBEGIN) add_tail(&head, line);       // Defer nozzle switching gcode
        else if (pos == FLUSH){
            flushlist(outputFile, &head); // Flush after printer moved to prime tower
            pos = NORMAL;
            fputs(line, outputFile);      // Don't forget current line
        }
        else                      fputs(line, outputFile);      // Copy lines out otherwise
    } //while

    fclose(inputFile);
    fclose(outputFile);
    exit(0);
}

/**
 * @brief      Looks for predefined keywords.
 *
 * @param      line  The buffer from @inputFile
 */
void parsecomment(char *line){
    if      ( strcmp(line, ";START\n")    == 0 )           pos = START;
    else if ( strcmp(line, ";MATBEGIN\n") == 0 )           pos = MATBEGIN;
    else if ( strcmp(line, ";MATEND\n") == 0 )             pos = MATEND;
    else if ( (line[1] == 'T') && (pos == MATEND) )      pos = FLUSH;   // Only CURA
    else if ( strcmp(line, ";END\n")      == 0 )           pos = END;
    else if ( pos >= START && pos < END )                pos = NORMAL;   // Any comment after start will change state to normal
}


/**
 * @brief      Determines how close you can place a prime tower.
 * 
 * This method will most likely break down when more than one object is on the bed.
 *
 * @param      line  The line to scrape
 */
void scrapecords(char *line){
    const char key[2] = " ";
    float num = 0;
    char* temppter = NULL;
    char linecpy[200];
    strcpy(linecpy, line);      // Strtok messes up strings

    char *feild = strtok(linecpy, key);

    while (feild != NULL){
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


/**
 * @brief      Adds a new node to the tail.
 *
 * @param      head  The head of the list.
 * @param[in]  val   The value to store.
 */
void add_tail(node_t *head, char *val){
    node_t *current = head->next;
    if (head->next != NULL){

        while (current->next != NULL){
            current = current->next;
        }
        current->next = malloc(sizeof(node_t));
        strcpy(current->next->data, val);
        current->next->next = NULL;
    }
    else{
        head->next = malloc(sizeof(node_t));
        strcpy(head->next->data, val);
        head->next->next = NULL;   
    }
}


/**
 * @brief      Dumps the list out before the current line.
 *
 * @param      outputFile  The output file
 * @param      head        The head of the list
 */
void flushlist(FILE *outputFile, node_t *head){
    node_t *temp = head->next;
    while (head->next != NULL){
        fputs(head->next->data, outputFile);
        head->next = head->next->next;
        free(temp);
        temp = head->next;
    }
}