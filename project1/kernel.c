/* ACADEMIC INTEGRITY PLEDGE                                              */
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
/* Signed:___________Douglas Keller____________ Date:__2/15/2016__        */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS kernel, Version 1.02, Spring 2016.                 */

void printString(char*);
void readString(char*);
void clearScreen(int, int);
void handleInterrupt21(int, int, int, int);

void writeInt(int);
void readInt(int*);

int mod(int, int);
int div(int, int);

void main() {
    char line[80];
    int x;

    makeInterrupt21();

    interrupt(33, 12, 4, 11, 0);

    interrupt(33, 0, "\r\n  oooooooooo.   oooo    oooo         oooooooooo.     .oooooo.    .oooooo..o\0", 0, 0);
    interrupt(33, 0, "\r\n  `888'   `Y8b  `888   .8P'          `888'   `Y8b   d8P'  `Y8b  d8P'    `Y8\0", 0, 0);
    interrupt(33, 0, "\r\n   888      888  888  d8'             888      888 888      888 Y88bo.     \0", 0, 0);
    interrupt(33, 0, "\r\n   888      888  88888[               888      888 888      888  `\"Y8888o. \0", 0, 0);
    interrupt(33, 0, "\r\n   888      888  888`88b.    8888888  888      888 888      888      `\"Y88b\0", 0, 0);
    interrupt(33, 0, "\r\n   888     d88'  888  `88b.           888     d88' `88b    d88' oo     .d8P\0", 0, 0);
    interrupt(33, 0, "\r\n  o888bood8P'   o888o  o888o         o888bood8P'    `Y8bood8P'  8""888888P' \0", 0, 0);     
    
    interrupt(33, 0, "\r\n", 0, 0);
    interrupt(33, 0, "\r\n  Author: Douglas Keller\0", 0, 0);
    interrupt(33, 0, "\r\n", 0, 0);

    interrupt(33, 0, "\r\nEnter a line: \0", 0, 0);
    interrupt(33, 1, line, 0, 0);
    interrupt(33, 0, "\r\nYou typed: \0", 0, 0);
    interrupt(33, 0, line, 0, 0);

    interrupt(33, 0, "\r\n\0", 0, 0);
    interrupt(33, 0, "Enter a number: \0", 0, 0);
    interrupt(33, 14, &x, 0, 0);
    interrupt(33, 0, "\r\n\0", 0, 0);
    interrupt(33, 0, "You entered \0", 0, 0);
    interrupt(33, 13, x, 0, 0);
    interrupt(33, 0, "\r\n\0", 0, 0);

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

void handleInterrupt21(int ax, int bx, int cx, int dx) {
    switch(ax) {
        case 0:
            printString(bx);
            break;
        case 1:
            readString(bx);
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
    }
}