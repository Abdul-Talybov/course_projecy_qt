#pragma once
#include "Key.h"

struct HashNode {
    Key* key;
    HashNode* next;
};
