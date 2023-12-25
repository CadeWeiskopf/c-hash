#include "LinkedList.h"
#include <stdlib.h>

LinkedList* LinkedListAllocate() {
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    return list;
}

LinkedList* LinkedListInsert(LinkedList* list, HashItem* item) {
    if (!list) {
        LinkedList* head = LinkedListAllocate();
        head->item = item;
        head->next = NULL;
        return head;
    } else if (list->next == NULL) {
        LinkedList* node = LinkedListAllocate();
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }

    LinkedList* temp = list;
    while (temp->next->next) {
        temp = temp->next;
    }
    LinkedList* node = LinkedListAllocate();
    node->item = item;
    node->next = NULL;
    temp->next = node;
    return list;
}

void LinkedListFree(LinkedList* list) {
    LinkedList* temp;
    while (list) {
        temp = list;
        list = list->next;
        free(temp->item->key);
        free(temp->item->value);
        free(temp->item);
        free(temp);
    }
}