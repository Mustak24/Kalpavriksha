#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

#include "commands.h"
#include "filesystem.h"
#include "common.h"



FunctionStatus sendFunctionStatus(bool success, const char* messageFstring, ...) {
    FunctionStatus status = {.success=success, .message=""};

    char message[FUNCTION_STATUS_MSG_MAX_LEN];

    va_list args;
    va_start(args, messageFstring);
    vsnprintf(message, sizeof(message), messageFstring, args);
    va_end(args);

    strcpy(status.message, message);

    return status;
}


void displayFunctionStatus(FunctionStatus status) {
    printf(
        "%s: %s\n", 
        status.success ? "SUCCESS" : "ERROR",
        status.message
    );
}


bool isValidFileNodeName(const char* name) {
    size_t nameLength = strlen(name);
    if(
        nameLength < FILE_NAME_MIN_LEN ||
        nameLength > FILE_NAME_MAX_LEN
    ) return false;

    char validChars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890._-";

    for(const char* ch = name; *ch != '\0'; ch++) {
        bool isValidCh = false;
        
        for(const char* validCh = validChars; *validCh != '\0'; validCh++) {
            if(*ch == *validCh) {
                isValidCh = true;
                break;
            }
        }

        if(isValidCh == false) return false;
    }

    return true;
}


char** createStringChunks(const char* string, size_t chunkSize, size_t* resultChunkCounts) {
    if(
        string == NULL ||
        chunkSize == 0 ||
        resultChunkCounts == NULL
    ) return NULL;

    size_t stringSize = strlen(string) + 1;
    *resultChunkCounts = (size_t)ceil((double)stringSize / chunkSize);

    char** chunks = (char**)calloc(*resultChunkCounts, sizeof(char*));
    if(chunks == NULL) return NULL;

    for(size_t index = 0; index < *resultChunkCounts; index++) {
        *(chunks + index) = (char*)malloc(chunkSize * sizeof(char));

        if(*(chunks + index) == NULL) {
            for(size_t freeIndex = 0; freeIndex < index; freeIndex++) {
                free(*(chunks + freeIndex));
            }
            free(chunks);

            return NULL;
        }
    }

    for(size_t chunkIndex = 0; chunkIndex < *resultChunkCounts; chunkIndex++) {
        strncpy(
            *(chunks + chunkIndex), 
            (string + chunkIndex * chunkSize), 
            chunkSize * sizeof(char)
        );
    }

    return chunks;
}


void destroyStringChunks(char*** chunks, size_t chunksCount) {
    for(size_t index = 0; index < chunksCount; index++) {
        free(*(*chunks + index));
    }

    free(*chunks);
    *chunks = NULL;
}


void swapGenericData(void* data1, void* data2, size_t dataSize) {
    void* temp = malloc(dataSize);
    if(temp == NULL) return;

    memcpy(temp, data1, dataSize);
    memcpy(data1, data2, dataSize);
    memcpy(data2, temp, dataSize);

    free(temp);
} 


void clearInputBuffer(void) {
    int bufferChar;
    while((bufferChar = getchar()) != '\n' && bufferChar != EOF);
}


bool readSize_t(size_t* result, size_t min, size_t max) {
    int scanResult = scanf("%zu", result);
    
    return (
        scanResult == 1 &&
        *result >= min &&
        *result <= max
    );
}


char* readLine(void) {
    size_t lineLength = 0;
    char* line = (char*)malloc(sizeof(char));
    if(line == NULL) return NULL;
    
    line[0] = '\0';
    bool isInputComplite = false;

    while(!isInputComplite) {
        char inputBuffer[INPUT_BUFFER_SIZE];
        if(fgets(inputBuffer, INPUT_BUFFER_SIZE, stdin) == NULL) {
            free(line);
            return NULL;
        }

        size_t inputBufferLength = strlen(inputBuffer);

        isInputComplite = (
            (inputBufferLength + 1) < INPUT_BUFFER_SIZE ||
            (inputBufferLength > 0 && inputBuffer[inputBufferLength-1] == '\n')
        );
        
        if(isInputComplite && inputBufferLength > 0 && inputBuffer[inputBufferLength - 1] == '\n') {
            inputBuffer[inputBufferLength - 1] = '\0';
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


void trimString(char* string) {
    if(string == NULL) return;

    size_t length = strlen(string);
    if(length == 0) return;

    size_t start = 0;
    while(start < length && isspace(string[start])) start++;

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
