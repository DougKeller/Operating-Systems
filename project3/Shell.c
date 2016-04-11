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

// void runCommand(char*);
int isCommand(char*, char*);
void printDir();

void main() {
    char input[80];

    interrupt(33, 12, 4, 11, 0);
    // When this line is uncommented, random characters are printed out
    // PRINTS("Welcome to DK-DOS!\r\n\r\n\0");

    while(1) {
        PRINTS("cxxxx][======> \0");

        // Get a command from the user
        SCANS(input);
        PRINTS("\r\n\0");

        if(isCommand("dir\0", input)) {
            printDir();
        }
    }
}

int isCommand(char* command, char* input) {
    int i = 0;
    while(input[i] && command[i]) {
        if(input[i] != command[i]) {
            return 0;
        }
        i += 1;
    }

    return command[i] == 0x0;
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