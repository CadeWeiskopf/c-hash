#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "HashItem.h"
#include "LinkedList.h"

typedef struct HashTable {
    HashItem** items;
    LinkedList** overflowBuckets;
    int size;
    int count;
} HashTable;

unsigned long HashFunction(char* str);

LinkedList** HashTableCreateOverflowBuckets(HashTable* table);

HashTable* HashTableCreate();

void HashTableFreeOverflowBuckets(HashTable* table);

void HashTableHandleCollision(HashTable* table, unsigned long index, HashItem* item);

void HashTableInsert(HashTable* table, char* key, char* value);

void HashTableFree(HashTable* table);

void HashTablePrint(HashTable* table);

char* HashTableSearch(HashTable* table, char* key);


#endif