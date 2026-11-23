#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "heap.h"
#include "common.h"


static void* getNode(Heap* heap, int index) {
    if(heap == NULL || index < 0 || index >= heap->capacity) return NULL;
    return (char*)heap->data + (index * heap->dataTypeSize);
}

static void* peak(Heap* heap) {
    if(heap == NULL || heap->size == 0) return NULL;
    return getNode(heap, 0);
}

static void bubbleDownHeap(Heap* heap, int index) {
    if(heap == NULL || index < 0 || index >= heap->size) return;

    while(true) {
        int leftIndex = index * 2 + 1;
        int rightIndex = index * 2 + 2;
        int targetIndex = index;

        if(leftIndex < heap->size && heap->compareValue(getNode(heap, targetIndex), getNode(heap, leftIndex)) > 0) {
            targetIndex = leftIndex;
        }

        if(rightIndex < heap->size && heap->compareValue(getNode(heap, targetIndex), getNode(heap, rightIndex)) > 0) {
            targetIndex = rightIndex;
        }

        if(targetIndex == index) return;
        
        swap(
            getNode(heap, index), 
            getNode(heap, targetIndex), 
            heap->dataTypeSize
        );

        index = targetIndex;
    }
}

static void bubbleUpHeap(Heap* heap, int index) {
    if(heap == NULL || index < 0 || index >= heap->size) return;

    while(index > 0) {
        int parentIndex = (index - 1) / 2;

        void* currentNode = getNode(heap, index);
        void* parentNode = getNode(heap, parentIndex);

        if(heap->compareValue(parentNode, currentNode) > 0) {
            swap(currentNode, parentNode, heap->dataTypeSize);
            index = parentIndex;
            continue;
        }

        break;
    }
}

static bool push(Heap* heap, void* value) {
    if(heap == NULL || value == NULL || heap->size >= heap->capacity) return false;

    memcpy(getNode(heap, heap->size), value, heap->dataTypeSize);
    
    heap->size += 1;
    bubbleUpHeap(heap, heap->size - 1);

    return true;
}

static void* pop(Heap* heap) {
    if(heap == NULL || heap->size == 0) return NULL;
    
    void* result = malloc(heap->dataTypeSize);
    if(result == NULL) return NULL;

    void* rootNode = getNode(heap, 0);
    void* lastNode = getNode(heap, heap->size - 1);

    memcpy(result, rootNode, heap->dataTypeSize);
    if(heap->size > 1) {
        swap(rootNode, lastNode, heap->dataTypeSize);
    }

    heap->size -= 1;
    if(heap->size > 0) {
        bubbleDownHeap(heap, 0);
    }

    return result;
}

Heap* initHeap(int capacity, int dataTypeSize, int (*compareValue)(const void* value1, const void* value2)) {
    if(capacity <= 0 || dataTypeSize <= 0 || compareValue == NULL) return NULL;

    Heap* heap = (Heap*)malloc(sizeof(Heap));
    if(heap == NULL) return NULL;

    heap->data = malloc(capacity * dataTypeSize);
    if(heap->data == NULL) {
        free(heap);
        return NULL;
    }

    heap->size = 0;
    heap->capacity = capacity;
    heap->compareValue = compareValue;
    heap->dataTypeSize = dataTypeSize;

    heap->peak = peak;
    heap->pop = pop;
    heap->push = push;

    return heap;
}

void destroyHeap(Heap** heap) {
    if(heap == NULL || *heap == NULL) return;
    if((*heap)->data != NULL) {
        free((*heap)->data);
        (*heap)->data = NULL;
    }
    free(*heap);
    *heap = NULL;
}
