#ifndef STACK
#define STACK


#include <stdbool.h>

#define STACK_GROW_FACTOR 2

typedef struct Stack {
    void* data;
    size_t length;
    size_t capacity;
    size_t dataTypeSize;

    bool (*push)(struct Stack* self, void* data);
    bool (*pop)(struct Stack* self);
    bool (*remove)(struct Stack* self, long index);
    const void* (*get)(struct Stack* self, long index);
} Stack;


Stack* initializeStack(size_t dataTypeSize);
void destroyStack(Stack** self);


#endif