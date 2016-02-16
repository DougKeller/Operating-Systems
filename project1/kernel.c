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

    clearScreen(4, 11);

    printString("\r\n  oooooooooo.   oooo    oooo         oooooooooo.     .oooooo.    .oooooo..o\0"); 
    printString("\r\n  `888'   `Y8b  `888   .8P'          `888'   `Y8b   d8P'  `Y8b  d8P'    `Y8\0"); 
    printString("\r\n   888      888  888  d8'             888      888 888      888 Y88bo.     \0"); 
    printString("\r\n   888      888  88888[               888      888 888      888  `\"Y8888o. \0"); 
    printString("\r\n   888      888  888`88b.    8888888  888      888 888      888      `\"Y88b\0"); 
    printString("\r\n   888     d88'  888  `88b.           888     d88' `88b    d88' oo     .d8P\0"); 
    printString("\r\n  o888bood8P'   o888o  o888o         o888bood8P'    `Y8bood8P'  8""888888P' \0"); 
          
    printString("\r\n");                
    printString("\r\nEnter a line: \0");
    readString(line);
    printString("\r\nYou typed: \0");
    printString(line);

    printString("\r\n\0");
    printString("Enter a number: \0");
    readInt(&x);
    printString("\r\n\0");
    printString("You entered \0");
    writeInt(x);

    printString("\r\n\0");

    while(1);
}

void printString(char* message) {
    int i = 0;

    // Output chars from the message until null is met
    while(message[i] != 0x0) {
        interrupt(16, 14 * 256 + message[i], 0, 0, 0);
        i++;
    }
}

void readString(char* message) {
    int index = 0, lastIndex = 79;
    char inputChar;

    while(index < lastIndex) {
        // Read the next character
        inputChar = interrupt(22, 0, 0, 0, 0);

        switch(inputChar) {
            case 0x8: // Backspace
                if(index > 0) {
                    index--;
                    message[index] = 0;
                    // Clear the previously printed char from the terminal
                    interrupt(16, 14 * 256 + inputChar, 0, 0, 0);
                    interrupt(16, 14 * 256 + ' ', 0, 0, 0);
                    interrupt(16, 14 * 256 + inputChar, 0, 0, 0);
                }
                break;
            case 0xD: // Enter
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

    printString("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\0");
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
        printString("0\0");
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
    printString(d);
}

void readInt(int* address) {
    char digits[5];
    int maxIndex = 4, index = -1, power = 1, sum = 0;
    char inputChar;

    while(index < maxIndex) {
        // Read the next character
        inputChar = interrupt(22, 0, 0, 0, 0);

        switch(inputChar) {
            case 0x8: // Backspace
                if(index > -1) {
                    digits[index] = 0;
                    index--;
                    // Clear the previously printed char from the terminal
                    interrupt(16, 14 * 256 + inputChar, 0, 0, 0);
                    interrupt(16, 14 * 256 + ' ', 0, 0, 0);
                    interrupt(16, 14 * 256 + inputChar, 0, 0, 0);
                }
                break;
            case 0xD: // Enter
                // Set maxIndex to -1, which will terminate the loop
                maxIndex = -1;
                break;
            default:
                // Verify input is an integer character
                if(inputChar >= 48 && inputChar < 58) {
                    index++;
                    digits[index] = inputChar;
                    interrupt(16, 14 * 256 + inputChar, 0, 0, 0);
                }
        }
    }

    // Convert the int array to a single int
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