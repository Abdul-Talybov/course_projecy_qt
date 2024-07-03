#include "HashTable.h"

HashTable::HashTable(int size) : size(size) {
    table = new HashNode * [size];
    for (int i = 0; i < size; i++) {
        table[i] = nullptr;
    }
}

int HashTable::hashFunction(std::string discipline_name) {
    int hash = 0;
    for (char c : discipline_name) {
        hash = (hash + c) % size;
    }
    return hash;
}

int HashTable::calculateAsciiSum(std::string str) {
    int sum = 0;
    for (char c : str) {
        sum += c;
    }
    return sum;
}

// Modified insert function
void HashTable::insert(Key* key) {
    int index = hashFunction(key->discipline_name);
    HashNode* newNode = new HashNode;
    newNode->key = key;
    newNode->next = nullptr;

    if (table[index] == nullptr || calculateAsciiSum(table[index]->key->discipline_name) > calculateAsciiSum(key->discipline_name)) {
        newNode->next = table[index];
        table[index] = newNode;
    } else {
        HashNode* current = table[index];
        while (current->next != nullptr && calculateAsciiSum(current->next->key->discipline_name) <= calculateAsciiSum(key->discipline_name)) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

Key* HashTable::search_for_hash(Key* key) {
    int index = hashFunction(key->discipline_name);
    HashNode* node = table[index];
    while (node != nullptr) {
        if (node->key->discipline_name == key->discipline_name) {
            return node->key;
        }
        node = node->next;
    }
    return nullptr;
}

bool HashTable::remove(Key* key) {
    int index = hashFunction(key->discipline_name);
    HashNode* node = table[index];
    HashNode* prev = nullptr;

    while (node != nullptr) {
        if (node->key->discipline_name == key->discipline_name &&
            node->key->teacher_name == key->teacher_name &&
            node->key->direction_code.BB == key->direction_code.BB &&
            node->key->direction_code.CC == key->direction_code.CC &&
            node->key->direction_code.DD == key->direction_code.DD) {

            if (prev == nullptr) {
                table[index] = node->next;
            } else {
                prev->next = node->next;
            }
            delete node;
            return true;
        }
        prev = node;
        node = node->next;
    }
    return false;
}


int HashTable::countElements() {
    int count = 0;
    for (int i = 0; i < size; i++) {
        HashNode* node = table[i];
        while (node != nullptr) {
            count++;
            node = node->next;
        }
    }
    return count;
}

bool flag_find = false;
bool HashTable::find(Key* key, std::string discpli_name){
    int index = hashFunction(discpli_name);
    HashNode* node = table[index];
    while (node != nullptr) {
        if (node->key->discipline_name == discpli_name) {
            flag_find = true;
        }
        node = node->next;
    }
    if (flag_find){
        flag_find=false;
        return true;
    }
    else{
        return false;
    }
}
