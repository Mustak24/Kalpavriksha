#include <stdio.h>
#include <stdbool.h>

#define MIN_INPUT_CHARS 1
#define MAX_INPUT_CHARS 20

#define MIN_ASCII_VALUE 97
#define MAX_ASCII_VALUE 121


void cleanInputBuffer() {
    int bufferChar;
    while((bufferChar = getchar()) != '\n' && bufferChar != EOF);
}

bool isValidCharacter(char character) {
    return MIN_ASCII_VALUE <= character && character <= MAX_ASCII_VALUE;
}


int readInputSize(int* size) {
    printf("Enter count of characters (between %d and %d): ", MIN_INPUT_CHARS, MAX_INPUT_CHARS);
    
    if(
        scanf("%d", size) != 1 || 
        *size < MIN_INPUT_CHARS || 
        *size > MAX_INPUT_CHARS
    ) {
        printf("Invalid input. Count must be a positive integer (%d-%d).\n", MIN_INPUT_CHARS, MAX_INPUT_CHARS);
        return 1;
    }

    cleanInputBuffer();
    return 0;
}


int readInputString(char* input, int count) {
    printf("Enter %d characters (between 'a' and 'y'): ", count);
    int readCount = 0;
    
    while(readCount < count) {
        if(scanf("%c", &input[readCount]) != 1) {
            printf("Error: failed to read input character.\n");
            return 1;
        }

        if(input[readCount] == ' ' || input[readCount] == '\n') {
            continue;
        }

        if(!isValidCharacter(input[readCount])) {
            printf(
                "Error: Invalid input, Character must be between '%c' and '%c'.\n", 
                (char)MIN_ASCII_VALUE, (char)MAX_ASCII_VALUE
            );
            return 1;
        }

        readCount++;
    }
    
    cleanInputBuffer();

    input[readCount] = '\0';
    return 0;
}


void displayStringWithAsciiValues(const char* input) {
    int sum = 0;
    for(int index = 0; input[index] != '\0'; index++) {
        sum += (int)input[index];
        printf(
            "%c\t%d\t%c\n", 
            input[index], (int)input[index], (char)(input[index] + 1)
        );
    }

    printf("Sum = %d\n", sum);
}


int main() {
    int count;
    if(readInputSize(&count) != 0) {
        printf("Error: Unable to read input size.\n");
        return 1;
    }
    
    char input[MAX_INPUT_CHARS + 1];
    if(readInputString(input, count) != 0) {
        printf("Error: Unable to read input string.\n");
        return 1;
    }

    printf("\nOUTPUT:\n");
    displayStringWithAsciiValues(input);

    return 0;
}