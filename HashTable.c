#include "HashTable.h"
#include "HashItem.h"
#include "LinkedList.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CAPACITY 50000

unsigned long HashFunction(char* str) {
    unsigned long i = 0;
    for (int j = 0; str[j]; j++) {
        i += str[j];
    }
    return i % CAPACITY;
}

LinkedList** HashTableCreateOverflowBuckets(HashTable* table) {
    LinkedList** overflowBuckets = (LinkedList**) calloc(table->size, sizeof(LinkedList*));
    for (int i = 0; i < table->size; i++) {
        overflowBuckets[i] = NULL;
    }
    return overflowBuckets;
}

HashTable* HashTableCreate() {
    HashTable* table = (HashTable*) malloc(sizeof(HashTable));
    table->size = CAPACITY;
    table->count = 0;
    table->items = (HashItem**) calloc(table->size, sizeof(HashItem*));
    for (int i = 0; i < table->size; i++) {
        table->items[i] = NULL;
    }
    table->overflowBuckets = HashTableCreateOverflowBuckets(table);
    return table;
}

void HashTableFreeOverflowBuckets(HashTable* table) {
    for (int i = 0; i < table->size; i++) {
        LinkedListFree(table->overflowBuckets[i]);
    }
    free(table->overflowBuckets);
}

void HashTableHandleCollision(HashTable* table, unsigned long index, HashItem* item) {
    LinkedList* head = table->overflowBuckets[index];
    table->overflowBuckets[index] = LinkedListInsert(head, item);
}

void HashTableInsert(HashTable* table, char* key, char* value) {
    HashItem* newItem = HashItemCreate(key, value);
    int index = HashFunction(key);
    HashItem* existingItem = table->items[index];
    if (existingItem == NULL) {
        if (table->count == table->size) {
            printf("Error: HashTable is full\n");
            HashItemFree(newItem);
            return;
        }
        table->items[index] = newItem;
        table->count++;
    } else {
        if (strcmp(existingItem->key, key) == 0) {
            strcpy(table->items[index]->value, value);
            return;
        } else {
            HashTableHandleCollision(table, index, newItem);
            return;
        }
    }
}

void HashTableFree(HashTable* table) {
    for (int i = 0; i < table->size; i++) {
        HashItem* item = table->items[i];
        if (item != NULL) {
            HashItemFree(item);
        }
    }
    HashTableFreeOverflowBuckets(table);
    free(table->items);
    free(table);
}

void HashTablePrint(HashTable* table) {
    printf("\nHash Table\n-------------------\n");
    for (int i = 0; i < table->size; i++) {
        if (table->items[i]) {
            printf(
                "Index:%d, Key:%s, Value:%s\n", 
                i, 
                table->items[i]->key, 
                table->items[i]->value
            );
            LinkedList* temp = table->overflowBuckets[i];
            while (temp) {
                printf(
                    "Index:%d, Key:%s, Value:%s\n", 
                    i, 
                    temp->item->key, 
                    temp->item->value
                );
                temp = temp->next;
            }
        }
    }
    printf("-------------------\n\n");
}

char* HashTableSearch(HashTable* table, char* key) {
    int index = HashFunction(key);
    HashItem* item = table->items[index];
    if (item != NULL) {
        if (strcmp(item->key, key) == 0) {
            printf("{\"%s\": \"%s\"}\n", key, item->value);
            return item->value;
        }

        LinkedList* overflow = table->overflowBuckets[index];
        while (overflow != NULL) {
            if (strcmp(overflow->item->key, key) == 0) {
                printf("{\"%s\": \"%s\"}\n", key, overflow->item->value);
                return overflow->item->value;
            }
            overflow = overflow->next;
        }
    }
    printf("Key does not exist: %s\n", key);
    return NULL;
}
