#include "blackdos.h"

// Interrupt 33 commands
// 0: printString    char* str
// 1: readString     char* buffer
// 2: readSector     char* buffer
//                   int   sector
// 3: readFile       char* fileName
//                   char* buffer
//                   int   size
// 4: runProgram     char* exe
//                   int   segment
// 5: stop
// 6: writeSector    char* buffer
//                   int   sector
// 7: deleteFile     char* fileName
// 8: writeFile      char* fileName
//                   char* data
//                   int   size
// 11: reboot
// 12: clearScreen   int   foreground
//                   int   background
// 13: writeInt      int   value
// 14: readInt       int   value
// 15: error         int   code

void getCommand(int*, int*, int*, int*);
int streql(char*, char*);
void printDir();

void main() {
    char input[80];
    int ax, bx, cx, dx;

    CLS;
    // PRINTS("Welcome to DK-DOS!\r\n\r\n\0");

    while(1) {
        PRINTS("cxxxx][======> \0");

        // Get a command from the user
        getCommand(&ax, &bx, &cx, &dx);

        // Execute the command
        if(ax >= 0) {
            interrupt(33, ax, bx, cx, dx);
        }
    }
}

void getCommand(int* ax, int* bx, int* cx, int* dx) {
    char input[80];
    int i = 0;

    *ax = *bx = *cx = *dx = 0;

    // Read the user's input
    SCANS(input);
    PRINTS("\r\n\0");

    // Read the first word of input
    while(input[i] != 0x0 && input[i] != ' ') {
        i += 1;
    }
    input[i] = 0x0;

    if(streql(input, "boot\0")) {
        *ax = 11;
    } else if(streql(input, "cls\0")) {
        *ax = 12;
        *bx = 4;
        *cx = 11;
    } else if(streql(input, "dir\0")) {
        // printDir();
        *ax = -1;
    } else {
        *ax = 15;
    }
}

int streql(char* a, char* b) {
    int i = 0;
    while(a[i] == b[i]) {
        if(a[i] == 0x0) {
            return 1;
        }

        i += 1;
    }

    return 0;
}

void printDir() {
    char directory[512], fileName[7];
    int i = 0, j = 0;

    interrupt(33, 2, directory, 2);

    for(i; i < 512; i += 32) {
        if(directory[i] >= 'a') {
            for(j = 0; j < 6; j += 1) {
                fileName[j] = directory[i + j];
            }
            fileName[6] = 0x0;
            PRINTS(fileName);
            PRINTS("\r\n\0");
        }
    }
}