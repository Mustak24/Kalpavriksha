#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#include "common.h"


void swap(void* value1, void* value2, int dataTypeSize) {
    if(value1 == NULL || value2 == NULL || dataTypeSize <= 0) return;

    void* temp = malloc(dataTypeSize);
    if(temp == NULL) return;

    memcpy(temp, value1, dataTypeSize);
    memcpy(value1, value2, dataTypeSize);
    memcpy(value2, temp, dataTypeSize);
    free(temp);
}


void clearInputBuffer(void) {
    int ch;
    while((ch = getchar()) != '\n' && ch != EOF);
}


void trimString(char* string) {
    if(string == NULL) return;

    size_t length = strlen(string);
    if(length == 0) return;

    size_t start = 0;
    while(start < length && isspace((unsigned char)string[start])) start++;

    if (start == length) {
        string[0] = '\0';
        return;
    }

    size_t end = length - 1;
    while (end > start && isspace((unsigned char)string[end])) {
        end--;
    }

    size_t newLength = end - start + 1;
    if (start > 0) {
        memmove(string, string + start, newLength);
    }
    
    string[newLength] = '\0';
}


char* readLine(void) {
    size_t lineLength = 0;
    char* line = (char*)malloc(sizeof(char));
    if(line == NULL) return NULL;
    
    line[0] = '\0';
    bool isInputComplete = false;

    while(!isInputComplete) {
        char inputBuffer[INPUT_BUFFER_SIZE];
        if(fgets(inputBuffer, INPUT_BUFFER_SIZE, stdin) == NULL) {
            if(lineLength > 0) {
                return line;
            }
            free(line);
            return NULL;
        }

        size_t inputBufferLength = strlen(inputBuffer);

        isInputComplete = (
            (inputBufferLength + 1) < INPUT_BUFFER_SIZE ||
            (inputBufferLength > 0 && inputBuffer[inputBufferLength - 1] == '\n')
        );
        
        if(inputBufferLength > 0 && inputBuffer[inputBufferLength - 1] == '\n') {
            inputBuffer[inputBufferLength - 1] = '\0';
            inputBufferLength--;
        }
       
        lineLength += inputBufferLength;

        char* temp = malloc((lineLength + 1) * sizeof(char));
        if(temp == NULL) {
            free(line);
            return NULL;
        }

        snprintf(temp, (lineLength + 1), "%s%s", line, inputBuffer);
        free(line);

        line = temp;
    }

    return line;
}


bool readInt(int* result, int min, int max) {
    if(result == NULL) return false;

    char* line = readLine();
    if(line == NULL) return false;

    trimString(line);
    if(strlen(line) == 0) {
        free(line);
        return false;
    }

    char* endptr = NULL;
    long val = strtol(line, &endptr, 10);
    while(endptr && *endptr != '\0' && isspace((unsigned char)*endptr)) {
        endptr++;
    }

    bool ok = (*endptr == '\0' && val >= min && val <= max);
    if(ok) {
        *result = (int)val;
    }

    free(line);
    return ok;
}


bool readFloat(float* result, float min, float max) {
    if(result == NULL) return false;

    char* line = readLine();
    if(line == NULL) return false;

    trimString(line);
    if(strlen(line) == 0) {
        free(line);
        return false;
    }

    char* endptr = NULL;
    float val = strtof(line, &endptr);
    while(endptr && *endptr != '\0' && isspace((unsigned char)*endptr)) {
        endptr++;
    }

    bool ok = (endptr != line && *endptr == '\0' && val >= min && val <= max);
    if(ok) {
        *result = val;
    }

    free(line);
    return ok;
}


bool readString(char* buffer, int maxLen) {
    if(buffer == NULL || maxLen <= 0) return false;

    char* line = readLine();
    if(line == NULL) return false;

    trimString(line);
    size_t length = strlen(line);
    
    if(length > 0 && length <= (size_t)maxLen) {
        strncpy(buffer, line, maxLen);
        buffer[maxLen] = '\0';
        free(line);
        return true;
    }

    free(line);
    return false;
}


int promptAndReadInt(const char* prompt, int min, int max) {
    int result;
    printf("%s", prompt);

    while(readInt(&result, min, max) == false) {
        if(feof(stdin)) break;
        printf("Invalid input, Please enter a value b/w %d and %d: ", min, max);
    }

    return result;
}


float promptAndReadFloat(const char* prompt, float min, float max) {
    float result;
    printf("%s", prompt);

    while(readFloat(&result, min, max) == false) {
        if(feof(stdin)) break;
        printf("Invalid input, Please enter a value b/w %.2f and %.2f: ", min, max);
    }

    return result;
}


char* promptAndReadString(const char* prompt, int maxLen) {
    if(maxLen <= 0) return NULL;
    char* result = (char*)malloc(maxLen + 1);
    if(result == NULL) return NULL;

    printf("%s", prompt);

    while(readString(result, maxLen) == false) {
        if(feof(stdin)) {
            free(result);
            return NULL;
        }
        printf("Please enter a valid string (max-length: %d): ", maxLen);
    }

    return result;
}
