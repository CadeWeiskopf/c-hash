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

typedef struct HashTable {
    HashItem** items;
    int size;
    int count;
} HashTable;

HashTable* HashTableCreate(int size) {
    HashTable* table = (HashTable*) malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (HashItem**) calloc(table->size, sizeof(HashItem*));
    for (int i = 0; i < table->size; i++) {
        table->items[i] = NULL;
    }
    return table;
}

void HashTableHandleCollision(HashTable* table, HashItem* item) {
    printf("TODO: handle collision\n");
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
            HashTableHandleCollision(table, newItem);
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

int main() {
    HashTable* hashTable = HashTableCreate(CAPACITY);
    HashTableInsert(hashTable, "Test", "1");
    HashTableInsert(hashTable, "cool", "2");
    HashTablePrint(hashTable);
    return 0;
}