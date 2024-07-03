#pragma once
#include "HashNode.h"
#include <string>

struct HashTable {
    HashNode** table;
    int size;

    HashTable(int size);
    int hashFunction(std::string discipline_name);
    void insert(Key* key);
    Key* search_for_hash(Key* key);
    bool remove(Key* key);
    int countElements();
    bool find(Key* key, std::string discpli_name);
    int calculateAsciiSum(std::string str);
};
