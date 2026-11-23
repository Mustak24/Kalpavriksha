#ifndef HEAP_UTILS_H
#define HEAP_UTILS_H

#include <stdbool.h>

typedef struct Heap {
    void* data;
    int dataTypeSize;
    int capacity;
    int size;

    int (*compareValue)(const void* value1, const void* value2);
    bool (*push)(struct Heap* self, void* value);
    void* (*pop)(struct Heap* self);
    void* (*peak)(struct Heap* self);
} Heap;


Heap* initHeap(int capacity, int dataTypeSize, int (*compareValue)(const void* value1, const void* value2));
void destroyHeap(Heap** heap);

#endif
