#ifndef SORTED_LIST_UTILS_H
#define SORTED_LIST_UTILS_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct SortedListNode {
    void* data;
    struct SortedListNode* next;
} SortedListNode;

typedef struct SortedList {
    SortedListNode* head;
    int dataTypeSize;
    int size;

    int (*compareValue)(const void* value1, const void* value2);
    bool (*insert)(struct SortedList* self, void* value);
    void* (*peak)(struct SortedList* self);
    void* (*get)(struct SortedList* self, int index);
} SortedList;

SortedList* initSortedList(int dataTypeSize, int (*compareValue)(const void* value1, const void* value2));
void destroySortedList(SortedList** list);

#endif
