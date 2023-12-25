#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "HashItem.h"

typedef struct LinkedList {
    HashItem* item;
    struct LinkedList* next;
} LinkedList;

LinkedList* LinkedListAllocate();

LinkedList* LinkedListInsert(LinkedList* list, HashItem* item);

void LinkedListFree(LinkedList* list);

#endif