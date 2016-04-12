#include "blackdos.h"

// Interrupt 33 commands
// 0: printString    char* str
// 1: readString     char* buffer
//                   int   maxLength (opt)
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

void execCommand(char*);
int isCommand(char*, char*);

void nextFileName(char*, char*, int*);
void validateFile(char*);

void printDir();
void copy(char*);
void delete(char*);
void echo(char*);
void run(char*);
void tweet(char*);
void type(char*);
void help();
void commandNotFound();

void main() {
    char input[80];

    while(1) {
        PRINTS("> \0");

        // Get a command from the user
        SCANS(input);
        PRINTS("\r\n\0");

        // Execute the command
        execCommand(input);
    }
}

void execCommand(char* command) {
    if(isCommand("boot\0", command)) {
        interrupt(33, 11, 0, 0, 0);
    }
    else if(isCommand("cls\0", command)) {
        interrupt(33, 12, 4, 11, 0);
    }
    else if(isCommand("dir\0", command)) {
        printDir();
    }
    else if(isCommand("copy\0", command)) {
        copy(command + 5);
    }
    else if(isCommand("del\0", command)) {
        delete(command + 4);
    }
    else if(isCommand("echo\0", command)) {
        echo(command + 5);
    }
    else if(isCommand("type\0", command)) {
        type(command + 5);
    }
    else if(isCommand("run\0", command)) {
        run(command + 4);
    }
    else if(isCommand("tweet\0", command)) {
        tweet(command + 6);
    }
    else if(isCommand("help\0", command)) {
        help();
    }
    else {
        commandNotFound();
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

void validateFile(char* fileName) {
    if(fileName[0] < 'a') {
        interrupt(33, 15, 2, 0, 0);
        END;
    }
}

void copy(char* args) {
    char fileA[7], fileB[7], buffer[13312];
    int i = 0, sectors;

    // Get name for fileA
    while(args[i] == ' ') args += 1;
    while(args[i] != 0x0 && args[i] != ' ') {
        if(i < 6) {
            fileA[i] = args[i];
        }
        i += 1;
    }
    args += i;
    if(i < 6) fileA[i] = 0x0;
    else fileA[6] = 0x0;
    i = 0;

    // Get name for fileB
    while(args[i] == ' ') args += 1;
    while(args[i] != 0x0 && args[i] != ' ') {
        if(i < 6) {
            fileB[i] = args[i];
        }
        i += 1;
    }
    args += i;
    if(i < 6) fileB[i] = 0x0;
    else fileB[6] = 0x0;
    i = 0;

    // Validate file names
    validateFile(fileA);
    validateFile(fileB);

    // Read fileA into buffer
    interrupt(33, 3, fileA, buffer, &sectors);

    // Write buffer to fileB
    interrupt(33, 8, fileB, buffer, sectors);
}

void delete(char* fileName) {
    validateFile(fileName);
    interrupt(33, 7, fileName, 0, 0);
}

void echo(char* message) {
    PRINTS(message);
    PRINTS("\r\n\0");
}

void run(char* exe) {
    validateFile(exe);
    interrupt(33, 4, exe, 4, 0);
}

void tweet(char* fileName) {
    char message[140];

    // Validate the fileName
    validateFile(fileName);

    // Get the message from the user
    PRINTS("Enter a message to tweet: \0");
    interrupt(33, 1, message, 140, 0);

    // Write the message to fileName
    interrupt(33, 8, fileName, message, 1);

    PRINTS("\r\n\0");
}

void type(char* fileName) {
    char buffer[13312];
    int size;

    // Validate the fileName
    validateFile(fileName);

    // Load the file and print it out
    interrupt(33, 3, fileName, buffer, &size);
    interrupt(33, 0, buffer, 0, 0);
    interrupt(33, 0, "\r\n\0", 0, 0);
}

void printDir() {
    char directory[512], fileName[7];
    int i, j;

    interrupt(33, 2, directory, 2, 0);

    for(i = 0; i < 512; i += 32) {
        if(directory[i] >= 'a') {
            for(j = 0; j < 6; j += 1) {
                fileName[j] = directory[i + j];
            }
            fileName[6] = 0x0;
            PRINTS("    \0");
            PRINTS(fileName);
            PRINTS("\r\n\0");
        }
    }
}

void help() {
    PRINTS("  COMMAND   ARG(S)\r\n\0");
    PRINTS("==================================================\r\n\0");
    PRINTS("  boot      (none)         - Reboots DK-DOS\r\n\0");
    PRINTS("  cls       (none)         - Clears the console\r\n\0");
    PRINTS("  copy      ARG1, ARG2     - Copies file ARG1 into a new file called ARG2\r\n\0");
    PRINTS("  del       ARG1           - Deletes a file named ARG1\r\n\0");
    PRINTS("  dir       (none)         - Displays files in the directory\r\n\0");
    PRINTS("  echo      ARG1           - Prints message ARG1 in the console\r\n\0");
    PRINTS("  help      (none)         - Displays command information\r\n\0");
    PRINTS("  run       ARG1           - Loads and runs an executable file named ARG1\r\n\0");
    PRINTS("  type      ARG1           - Loads file ARG1 and prints it in the console\r\n\0");
    PRINTS("  tweet     ARG1           - Prompts for a message, then stores that\r\n\0");
    PRINTS("                               message in a file named ARG1\r\n\0");
}

void commandNotFound() {
    interrupt(33, 15, 50, 0, 0);
    END;
}