#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "sortedList.h"

static bool insert(SortedList* self, void* value) {
    if(self == NULL || value == NULL || self->compareValue == NULL) return false;

    SortedListNode* newNode = (SortedListNode*)malloc(sizeof(SortedListNode));
    if(newNode == NULL) return false;

    newNode->data = malloc(self->dataTypeSize);
    if(newNode->data == NULL) {
        free(newNode);
        return false;
    }

    memcpy(newNode->data, value, self->dataTypeSize);
    newNode->next = NULL;

    if(self->head == NULL) {
        self->head = newNode;
        self->size++;
        return true;
    }

    if(self->compareValue(value, self->head->data) > 0) {
        newNode->next = self->head;
        self->head = newNode;
        self->size++;
        return true;
    }

    SortedListNode* currentNode = self->head;
    while(currentNode->next != NULL && self->compareValue(value, currentNode->next->data) <= 0) {
        currentNode = currentNode->next;
    }

    newNode->next = currentNode->next;
    currentNode->next = newNode;
    self->size++;

    return true;
}

static void* peak(SortedList* self) {
    if(self == NULL || self->head == NULL) return NULL;
    return self->head->data;
}

static void* get(SortedList* self, int targetIndex) {
    if(self == NULL || targetIndex < 0 || targetIndex >= self->size) return NULL;

    SortedListNode* currentNode = self->head;
    for(int nodeIndex = 0; nodeIndex < targetIndex && currentNode != NULL; nodeIndex++) {
        currentNode = currentNode->next;
    }

    return currentNode != NULL ? currentNode->data : NULL;
}

SortedList* initSortedList(int dataTypeSize, int (*compareValue)(const void* value1, const void* value2)) {
    if(dataTypeSize <= 0 || compareValue == NULL) return NULL;

    SortedList* list = (SortedList*)malloc(sizeof(SortedList));
    if(list == NULL) return NULL;

    list->head = NULL;
    list->dataTypeSize = dataTypeSize;
    list->size = 0;
    list->compareValue = compareValue;

    list->insert = insert;
    list->peak = peak;
    list->get = get;

    return list;
}

void destroySortedList(SortedList** list) {
    if(list == NULL || *list == NULL) return;

    SortedListNode* currentNode = (*list)->head;
    while(currentNode != NULL) {
        SortedListNode* nextNode = currentNode->next;
        if(currentNode->data != NULL) {
            free(currentNode->data);
            currentNode->data = NULL;
        }
        free(currentNode);
        currentNode = nextNode;
    }

    free(*list);
    *list = NULL;
}
