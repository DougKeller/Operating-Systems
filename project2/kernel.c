﻿/* ACADEMIC INTEGRITY PLEDGE                                              */
/*                                                                        */
/* - I have not used source code obtained from another student nor        */
/*   any other unauthorized source, either modified or unmodified.        */
/*                                                                        */
/* - All source code and documentation used in my program is either       */
/*   my original work or was derived by me from the source code           */
/*   published in the textbook for this course or presented in            */
/*   class.                                                               */
/*                                                                        */
/* - I have not discussed coding details about this project with          */
/*   anyone other than my instructor. I understand that I may discuss     */
/*   the concepts of this program with other students and that another    */
/*   student may help me debug my program so long as neither of us        */
/*   writes anything during the discussion or modifies any computer       */
/*   file during the discussion.                                          */
/*                                                                        */
/* - I have violated neither the spirit nor letter of these restrictions. */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/* Signed:___________Douglas Keller____________ Date:__3/15/2016__        */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS kernel, Version 1.02, Spring 2016.                 */

void printString(char*);
void readString(char*);
void clearScreen(int, int);
void handleInterrupt21(int, int, int, int);
void readSector(char*, int);
void readFile(char*, char*, int*);
void runProgram(char*, int);

void error(int);

void writeInt(int);
void readInt(int*);

int mod(int, int);
int div(int, int);

void main() {
    char input[80];
    char buffer1[512];
    char buffer2[512];
    char buffer3[512];
    int size, i;
    makeInterrupt21();

    interrupt(33, 12, 4, 11, 0);

    interrupt(33, 0, "\r\n  oooooooooo.   oooo    oooo         oooooooooo.     .oooooo.    .oooooo..o\0", 0, 0);
    interrupt(33, 0, "\r\n  `888'   `Y8b  `888   .8P'          `888'   `Y8b   d8P'  `Y8b  d8P'    `Y8\0", 0, 0);
    interrupt(33, 0, "\r\n   888      888  888  d8'             888      888 888      888 Y88bo.     \0", 0, 0);
    interrupt(33, 0, "\r\n   888      888  88888[               888      888 888      888  `\"Y8888o. \0", 0, 0);
    interrupt(33, 0, "\r\n   888      888  888`88b.    8888888  888      888 888      888      `\"Y88b\0", 0, 0);
    interrupt(33, 0, "\r\n   888     d88'  888  `88b.           888     d88' `88b    d88' oo     .d8P\0", 0, 0);
    interrupt(33, 0, "\r\n  o888bood8P'   o888o  o888o         o888bood8P'    `Y8bood8P'  8""888888P' \0", 0, 0);     
    interrupt(33, 0, "\r\n\0");
    interrupt(33, 0, "\r\n\0");

    interrupt(33, 0, "Press enter to begin...\0", 0, 0);
    interrupt(33, 1, input, 0, 0);
    interrupt(33, 12, 4, 11, 0);

    // readSector demo
    interrupt(33, 0, "Reading the directory sector:\r\n\0", 0, 0);
    interrupt(33, 2, buffer1, 2, 0);
    for(i = 0; i < 512; i += 1) {
        interrupt(16, 14 * 256 + buffer1[i], 0, 0, 0);
    }
    interrupt(33, 0, "\r\n\0");
    interrupt(33, 0, "Press enter to continue...\0", 0, 0);
    interrupt(33, 1, input, 0, 0);
    interrupt(33, 12, 4, 11, 0);

    // readFile demo (missing file)
    interrupt(33, 0, "Attempting to read a missing file named 'text':\r\n\0", 0, 0);
    interrupt(33, 3, "text\0", 0, 0);
    interrupt(33, 0, "\r\n\0");
    interrupt(33, 0, "Press enter to continue...\0", 0, 0);
    interrupt(33, 1, input, 0, 0);
    interrupt(33, 12, 4, 11, 0);

    // readFile demo (1 sector)
    interrupt(33, 0, "Reading and printing a single-sector file named 'msg':\r\n\0", 0, 0);
    interrupt(33, 3, "msg\0", buffer2, &size);
    interrupt(33, 0, buffer2, 0, 0);
    interrupt(33, 0, "Size: \0", 0, 0);
    interrupt(16, 14 * 256 + size + '0', 0, 0, 0);
    interrupt(33, 0, "\r\n\r\n\0");
    interrupt(33, 0, "Press enter to continue...\0", 0, 0);
    interrupt(33, 1, input, 0, 0);
    interrupt(33, 12, 4, 11, 0);

    // readFile demo (2 sectors)
    size = 0;
    interrupt(33, 0, "Reading and printing a multi-sector file named 'lonmsg':\r\n\0", 0, 0);
    interrupt(33, 3, "lonmsg\0", buffer3, &size);
    interrupt(33, 0, buffer3, 0, 0);
    interrupt(33, 0, "Size: \0", 0, 0);
    interrupt(16, 14 * 256 + size + '0', 0, 0, 0);
    interrupt(33, 0, "\r\n\r\n\0", 0, 0);
    interrupt(33, 0, "Press enter to continue...\0", 0, 0);
    interrupt(33, 1, input, 0, 0);
    interrupt(33, 12, 4, 11, 0);

    // runProgram demo
    interrupt(33, 0, "Running the fib program:\r\n\0", 0, 0);
    interrupt(33, 4, "fib\0", 2, 0);

    while(1);
}

void printString(char* message) {
    int i = 0;

    /* Output chars from the message until null is met */
    while(message[i] != 0x0) {
        interrupt(16, 14 * 256 + message[i], 0, 0, 0);
        i++;
    }
}

void readString(char* message) {
    int index = 0, lastIndex = 79;
    char inputChar;

    while(index < lastIndex) {
        /* Read the next character */
        inputChar = interrupt(22, 0, 0, 0, 0);

        switch(inputChar) {
            case 0x8: /* Backspace */
                if(index > 0) {
                    index--;
                    message[index] = 0;
                    /* Clear the previously printed char from the terminal */
                    interrupt(16, 14 * 256 + inputChar, 0, 0, 0);
                    interrupt(16, 14 * 256 + ' ', 0, 0, 0);
                    interrupt(16, 14 * 256 + inputChar, 0, 0, 0);
                }
                break;
            case 0xD: /* Enter */
                message[index] = 0x0;
                return;
            default:
                message[index] = inputChar;
                index++;
                interrupt(16, 14 * 256 + inputChar, 0, 0, 0);
        }

    }

    message[lastIndex] = 0x0;
}

void clearScreen(int bx, int cx) {
    int i = 0;
    int bh = 4096 * (bx - 1) + 256 * (cx - 1);

    interrupt(33, 0, "\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\0", 0, 0);
    interrupt(16, 512, 0, 0, 0);

    if(bx > 0 && bx <= 8 && cx > 0 && cx <= 16) {
        interrupt(16, 1536, bh, 0, 6223);
    }
}

int mod(int a, int b) {
    while(a >= b) {
        a -= b;
    }
    return a;
}

int div(int a, int b) {
    int q = 0;
    while(q * b <= a) {
        q++;
    }
    return q - 1;
}

void writeInt(int x) {
    char number[6];
    char* d;
    int q = x, r;

    if(q == 0) {
        interrupt(33, 0, "0\0", 0, 0);
        return;
    }

    d = number + 5;
    *d = 0;
    d--;

    while(q > 0) {
        r = mod(q, 10);
        q = div(q, 10);
        *d = r + 48;
        d--;
    }

    d++;
    interrupt(33, 0, d, 0, 0);
}

void readInt(int* address) {
    char digits[5];
    int maxIndex = 4, index = -1, power = 1, sum = 0;
    char inputChar;

    while(index < maxIndex) {
        /* Read the next character */
        inputChar = interrupt(22, 0, 0, 0, 0);

        switch(inputChar) {
            case 0x8: /* Backspace */
                if(index > -1) {
                    digits[index] = 0;
                    index--;
                    /* Clear the previously printed char from the terminal */
                    interrupt(16, 14 * 256 + inputChar, 0, 0, 0);
                    interrupt(16, 14 * 256 + ' ', 0, 0, 0);
                    interrupt(16, 14 * 256 + inputChar, 0, 0, 0);
                }
                break;
            case 0xD: /* Enter */
                /* Set maxIndex to -1, which will terminate the loop */
                maxIndex = -1;
                break;
            default:
                /* Verify input is an integer character */
                if(inputChar >= 48 && inputChar < 58) {
                    index++;
                    digits[index] = inputChar;
                    interrupt(16, 14 * 256 + inputChar, 0, 0, 0);
                }
        }
    }

    /* Convert the int array to a single int */
    while(index >= 0) {
        sum += (digits[index] - '0') * power;
        power *= 10;
        index--;
    }

    *address = sum;
}

void readSector(char* address, int sector) {
    int relativeSector = mod(sector, 18) + 1;
    int head = mod(div(sector, 16), 2);
    int track = div(sector, 36);
    int ax = 513;
    int cx = track * 256 + relativeSector;
    int dx = head * 256;

    // Load calculated sector into the provided address
    interrupt(19, 513, address, cx, dx);
}

void readFile(char* fileName, char* buffer, int* size) {
    char directory[512];
    int i, j;
    int sectorCount = 0;

    readSector(directory, 2);

    for(i = 0; i < 512; i += 32) {
        // Iterate through every file name in the directory

        if(directory[i] == 0x0) {
            // Skip this iteration since there is no file
            continue;
        }

        // Check if the file names match
        for(j = 0; j < 6; j += 1) {
            if(fileName[j] == '\0') {
                // End of fileName is reached; file found
                j = 6;
                break;
            } else if(directory[i + j] != fileName[j]) {
                // Names do not match
                break;
            }
        }

        if(j == 6) {
            // File names match
            break;
        }
    }

    if(i == 512) {
        // File not found
        error(0);
    } else {
        // Filename is at i'th index of directory sector
        j += i;
        for(j; j < i + 32; j += 1) {
            // Read sector numbers of the file
            if(directory[j] == 0) {
                break;
            }

            readSector(buffer, directory[j]);
            buffer += 512;
            sectorCount += 1;
        }

        *size = sectorCount;
    }
}

void runProgram(char* name, int segment) {
    char buffer[512];
    int size = 0, baseLoc = segment * 4096;
    int offset;
    
    // Read program file into buffer
    readFile(name, buffer, &size);

    if(size == 0) {
        // Return if file is not found
        return;
    }

    for(offset = 0; offset < size * 512; offset += 1) {
        // Put each char in the buffer into memory
        putInMemory(baseLoc, offset, buffer[offset]);
    }

    // Launch the program
    launchProgram(baseLoc);
}

void stop() {
    while(1);
}

void error(bx) {
    switch(bx) {
        case 0:
            printString("File not found\r\n\0");
            break;
        default:
            printString("General error\r\n\0");
            break;
    }
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
    switch(ax) {
        case 0:
            printString(bx);
            break;
        case 1:
            readString(bx);
            break;
        case 2:
            readSector(bx, cx);
            break;
        case 3:
            readFile(bx, cx, dx);
            break;
        case 4:
            runProgram(bx, cx);
            break;
        case 5:
            stop();
            break;
        case 12:
            clearScreen(bx, cx);
            break;
        case 13:
            writeInt(bx);
            break;
        case 14:
            readInt(bx);
            break;
        case 15:
            error(bx);
            break;
    }
}
