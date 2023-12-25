#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 50000

unsigned long HashFunction(char* str) {
    unsigned long i = 0;
    for (int j = 0; str[j]; j++) {
        i += str[j];
    }
    return i % CAPACITY;
}

typedef struct HashItem {
    char* key;
    char* value;
} HashItem;

HashItem* HashItemCreate(char* key, char* value) {
    HashItem* item = (HashItem*) malloc(sizeof(HashItem));
    item->key = (char*) malloc(strlen(key) + 1);
    item->value = (char*) malloc(strlen(value) + 1);
    strcpy(item->key, key);
    strcpy(item->value, value);
    return item;
}

void HashItemFree(HashItem* item) {
    free(item->key);
    free(item->value);
    free(item);
}

typedef struct LinkedList {
    HashItem* item;
    struct LinkedList* next;
} LinkedList;

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

typedef struct HashTable {
    HashItem** items;
    LinkedList** overflowBuckets;
    int size;
    int count;
} HashTable;

LinkedList** HashTableCreateOverflowBuckets(HashTable* table) {
    LinkedList** overflowBuckets = (LinkedList**) calloc(table->size, sizeof(LinkedList*));
    for (int i = 0; i < table->size; i++) {
        overflowBuckets[i] = NULL;
    }
    return overflowBuckets;
}

HashTable* HashTableCreate(int size) {
    HashTable* table = (HashTable*) malloc(sizeof(HashTable));
    table->size = size;
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
                table->items[i] -> key, 
                table->items[i]->value
            );
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

int main() {
    HashTable* hashTable = HashTableCreate(CAPACITY);
    
    HashTableInsert(hashTable, "Test", "1");
    HashTableInsert(hashTable, "d", "2");
    HashTableInsert(hashTable, "22", "3");
    HashTableInsert(hashTable, "(<", "4");
    HashTableInsert(hashTable, " D", "5");

    HashTableSearch(hashTable, "Test");
    HashTableSearch(hashTable, "d");
    HashTableSearch(hashTable, "22");
    HashTableSearch(hashTable, "(<");
    HashTableSearch(hashTable, " D");

    HashTablePrint(hashTable);

    return 0;
}