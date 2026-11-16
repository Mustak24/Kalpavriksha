#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "stack.h"
#include "common.h"


static bool stackPush(Stack* stack, void* data) {
    if(stack == NULL || data == NULL) return false;

    if(stack->length == stack->capacity) {
        if((stack->capacity * stack->dataTypeSize) > (SIZE_MAX / STACK_GROW_FACTOR)) return false;

        void* tempData = realloc(stack->data, STACK_GROW_FACTOR * stack->capacity * stack->dataTypeSize);
        if(tempData == NULL) return false;

        stack->data = tempData;
        stack->capacity *= STACK_GROW_FACTOR;
    }

    const char* sourceAddress = (const char*)data;
    const char* baseAddress = (const char*)(stack->data);
    char* destinationAddress = (char*)(baseAddress + (stack->length * stack->dataTypeSize));

    memmove(destinationAddress, sourceAddress, stack->dataTypeSize);
    stack->length += 1;
    return true;
}


static bool stackPop(Stack* stack) {
    if(stack == NULL || stack->length == 0) return false;

    memset(
        (char*)stack->data + (stack->length - 1) * stack->dataTypeSize,
        0,
        stack->dataTypeSize
    );
    
    stack->length -= 1;
    return true;
}


static const void* stackGet(Stack* stack, long index) {
    if(
        stack == NULL || 
        stack->length == 0 ||
        index >= (long)stack->length ||
        index < -(long)stack->length 
    ) return NULL;

    if(index < 0) {
        index += (long)stack->length;
    }

    return (void*)((char*)(stack->data) + index * stack->dataTypeSize);
}

static bool stackRemove(Stack* stack, long index) {
    if(
        stack == NULL ||
        stack->length == 0 ||
        index >= (long)stack->length ||
        index < -(long)stack->length 
    ) return false;

    if(index < 0) {
        index += (long)stack->length;
    }

    for(size_t shiftIndex = (size_t)index + 1; shiftIndex < stack->length; shiftIndex++) {
        swapGenericData(
            (void*)stack->get(stack, shiftIndex - 1),
            (void*)stack->get(stack, shiftIndex),
            stack->dataTypeSize
        );
    }

    stack->pop(stack);
    return true;
}


Stack* initializeStack(size_t dataTypeSize) {
    if(dataTypeSize <= 0) return NULL;

    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if(stack == NULL) return NULL;

    stack->data = malloc(dataTypeSize);
    if(stack->data == NULL) {
        free(stack);
        return NULL;
    }

    stack->length = 0;
    stack->capacity = 1;
    stack->dataTypeSize = dataTypeSize;

    stack->pop = stackPop;
    stack->push = stackPush;
    stack->remove = stackRemove;
    stack->get = stackGet;

    return stack;
}


void destroyStack(Stack** stack) {
    if(stack == NULL || *stack == NULL) return;

    free((*stack)->data);
    free(*stack);
    *stack = NULL;
}