#ifndef HASH_ITEM_H
#define HASH_ITEM_H

#include "HashItem.h"

typedef struct HashItem {
    char* key;
    char* value;
} HashItem;

HashItem* HashItemCreate(char* key, char* value);

void HashItemFree(HashItem* item);

#endif