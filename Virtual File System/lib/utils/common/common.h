#ifndef COMMON_UTILS
#define COMMON_UTILS


#include <stdbool.h>

#define INPUT_BUFFER_SIZE 1024
#define FUNCTION_STATUS_MSG_MAX_LEN 1024

#define STRINGIFY_IMPL(value) #value
#define STRINGIFY(value) STRINGIFY_IMPL(value)

typedef struct FunctionStatus {
    bool success;
    char message[FUNCTION_STATUS_MSG_MAX_LEN];
} FunctionStatus;

FunctionStatus sendFunctionStatus(bool success, const char* messageFstring, ...);
void displayFunctionStatus(FunctionStatus status);


bool isValidFileNodeName(const char* name);

char** createStringChunks(const char* string, size_t chunkSize, size_t* resultChunkCounts);
void destroyStringChunks(char*** chunks, size_t chunksCount);

void swapGenericData(void* data1, void* data2, size_t dataSize);
void clearInputBuffer(void);

bool readSize_t(size_t* result, size_t min, size_t max);
void trimString(char* string);

char* readLine(void);


#endif