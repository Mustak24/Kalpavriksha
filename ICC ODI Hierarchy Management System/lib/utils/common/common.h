#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <stdbool.h>

#define INPUT_BUFFER_SIZE 1024

void swap(void* value1, void* value2, int dataTypeSize);
void clearInputBuffer(void);
void trimString(char* string);

char* readLine(void);
bool readInt(int* result, int min, int max);
bool readFloat(float* result, float min, float max);
bool readString(char* buffer, int maxLen);

int promptAndReadInt(const char* prompt, int min, int max);
float promptAndReadFloat(const char* prompt, float min, float max);
char* promptAndReadString(const char* prompt, int maxLen);

#endif
