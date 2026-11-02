#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MATRIX_MIN_SIZE 2
#define MATRIX_MAX_SIZE 10
#define MATRIX_MIN_VALUE 0
#define MATRIX_MAX_VALUE 255

#define FILTER_SIZE 3


void seedTime() {
    srand(time(NULL));
}

int generateRandomInt(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void cleanInputBuffer() {
    int bufferChar;
    while((bufferChar = getchar()) != '\n' && bufferChar != EOF);
}

void swap(int* num1, int* num2) {
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}


void destroyMatrix(int** matrix, int size) {
    if(matrix == NULL) return;

    for(int row = 0; row < size; row++) {
        free(*(matrix + row));
    }

    free(matrix);
}


int** initializeMatrix(int size) {
    int** matrix = (int**)calloc(size, sizeof(int*));
    if(matrix == NULL) {
        printf("Memory allocation failed for matrix rows.\n");
        return NULL;
    }

    for(int row = 0; row < size; row++) {
        *(matrix + row) = (int*)malloc(size * sizeof(int));

        if(*(matrix + row) == NULL) {
            printf("Memory allocation failed for matrix columns.\n");
            destroyMatrix(matrix, row);

            return NULL;
        }
    }

    return matrix;
}


void fillMatrixWithRandomValues(int** matrix, int size) {
    for(int row = 0; row < size; row++) {
        for(int col = 0; col < size; col++) {
            *(*(matrix + row) + col) = generateRandomInt(
                MATRIX_MIN_VALUE, 
                MATRIX_MAX_VALUE
            );
        }
    }
}


void displayMatrix(int** matrix, int size) {
    for(int row = 0; row < size; row++) {
        for(int col = 0; col < size; col++) {
            printf("%d\t", *(*(matrix + row) + col));
        }
        printf("\n");
    }
}


void reverseArray(int* arr, int size) {
    int start = 0, end = size - 1;
    while(start < end) {
        swap(arr + start, arr + end);
        start++;
        end--;
    }
}

void rotateMatrix90(int** matrix, int size) {
    if(matrix == NULL || size < MATRIX_MIN_SIZE || size > MATRIX_MAX_SIZE) {
        printf("Invalid matrix provided for rotation.\n");
        return;
    }
    
    for(int row = 0; row < size; row++) {
        for(int col = row + 1; col < size; col++) {
            swap(
                (*(matrix + row) + col),
                (*(matrix + col) + row)
            );
        }
    }

    for(int row = 0; row < size; row++) {
        reverseArray(*(matrix + row), size);
    }
}



int calSmoothingFilterVal(
    int* filterRows[FILTER_SIZE],
    int rowSize,
    int filterApplyIndex
) {
    int offset = FILTER_SIZE / 2;
    int sum = 0, count = 0;
    
    for(int row = 0; row < FILTER_SIZE; row++) {
        if(*(filterRows + row) == NULL) continue;

        for(int col = filterApplyIndex - offset; col <= filterApplyIndex + offset; col++) {
            if(col < 0 || col >= rowSize) continue;

            sum += *(*(filterRows + row) + col);
            count++;
        }
    }
    
    return sum / count;
}


void applySmoothingFilterOnMatrix(int** matrix, int size) {
    if(matrix == NULL || size < MATRIX_MIN_SIZE || size > MATRIX_MAX_SIZE) {
        printf("Invalid matrix provided.\n");
        return;
    }

    int* previousRow = (int*)malloc(size * sizeof(int));
    int* currentRow = (int*)malloc(size * sizeof(int));

    if(previousRow == NULL || currentRow == NULL) {
        printf("Memory allocation failed for filter application.\n");
        free(previousRow);
        free(currentRow);
        return;
    }

    for(int colIndex = 0; colIndex < size; colIndex++) {
        *(previousRow + colIndex) = 0;
        *(currentRow + colIndex) = *(*(matrix + 0) + colIndex);
    }

    for(int rowIndex = 0; rowIndex < size; rowIndex++) {
        int* filterRows[FILTER_SIZE] = {
            rowIndex > 0 ? previousRow : NULL, 
            currentRow, 
            (rowIndex + 1 < size) ? *(matrix + rowIndex + 1) : NULL
        };

        for(int colIndex = 0; colIndex < size; colIndex++) {
            *(*(matrix + rowIndex) + colIndex) = calSmoothingFilterVal(
                filterRows,
                size, 
                colIndex
            );
        }

        for(int colIndex = 0; colIndex < size; colIndex++) {
            *(previousRow + colIndex) = *(currentRow + colIndex);

            if(rowIndex + 1 < size) {
                *(currentRow + colIndex) = *(*(matrix + rowIndex + 1) + colIndex);
            }
        }
    }

    free(previousRow);
    free(currentRow);
}


int readMatrixSize(int* size) {
    if(size == NULL) return 1;
    
    printf("Enter the size of the matrix (between %d and %d): ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);

    while(
        scanf("%d", size) != 1 || 
        *size < MATRIX_MIN_SIZE || 
        *size > MATRIX_MAX_SIZE
    ) {
        if(feof(stdin)) {
            printf("Error: Input ended unexpectedly\n");
            return 1;
        }

        printf("Warning, Please enter a valid number between %d and %d: ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
        cleanInputBuffer();
    }

    cleanInputBuffer();

    return 0;
}


int main() {
    seedTime();

    int size;
    int error = readMatrixSize(&size);

    if(error != 0) {
        printf("Failed to read matrix size.\n");
        return 1;
    }
    
    int** matrix = initializeMatrix(size);
    if(matrix == NULL) {
        printf("Failed to initialize matrix.\n");
        return 1;
    }

    printf("\nOriginal Matrix:\n");
    fillMatrixWithRandomValues(matrix, size);
    displayMatrix(matrix, size);

    printf("\nMatrix after 90-degree rotation:\n");
    rotateMatrix90(matrix, size);
    displayMatrix(matrix, size);

    printf("\nMatrix after applying smoothing filter:\n");
    applySmoothingFilterOnMatrix(matrix, size);
    displayMatrix(matrix, size);

    destroyMatrix(matrix, size);
    return 0;
}