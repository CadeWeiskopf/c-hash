#include "HashItem.h"
#include <stdlib.h>
#include <string.h>

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