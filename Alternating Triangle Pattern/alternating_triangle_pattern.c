#include <stdio.h>

#define TRIANGLE_MIN_ROW 1
#define TRIANGLE_MAX_ROW 10


int readRowCount(int* rowCount) {
    printf(
        "Enter the number of rows for the triangle (between %d and %d): ", 
        TRIANGLE_MIN_ROW, 
        TRIANGLE_MAX_ROW
    );

    if(
        scanf("%d", rowCount) != 1 || 
        *rowCount < TRIANGLE_MIN_ROW || 
        *rowCount > TRIANGLE_MAX_ROW
    ) {
        printf(
            "Error: Invalid input, Number of rows must be a positive integer (%d-%d).\n", 
            TRIANGLE_MIN_ROW, 
            TRIANGLE_MAX_ROW
        );
        return 1;
    }

    return 0;
}


char getSymbol(int rowIndex) {
    if(rowIndex % 2 == 0) {
        return '*';
    } else {
        return '#';
    }
}

void displayTriangle(int rowCount) {
    for(int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
        char symbol = getSymbol(rowIndex);

        for(int colIndex = 0; colIndex <= rowIndex; colIndex++) {
            printf("%c", symbol);
        }

        printf("\n");
    }
}


int main() {
    int rowCount;
    if(readRowCount(&rowCount) != 0) {
        printf("Error: failed to read the number of rows for the triangle.\n");
        return 1;
    }
   
    displayTriangle(rowCount);
    return 0;
}