#include <stdio.h>
#include "HashTable.h"

int main() {
    HashTable* hashTable = HashTableCreate();
    
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