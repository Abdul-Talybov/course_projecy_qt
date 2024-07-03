#include "Knot.h"

Knot::Knot(int tableSize) : key(new Key()), hashTable(new HashTable(tableSize)), right(nullptr), left(nullptr), bal(0), headp(nullptr) {}
