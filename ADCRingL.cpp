#include "ADCRingL.h"
#include <iostream>

Node* init(int data) {
    Node* head = new Node();
    head->data = data;
    head->next = head;
    head->prev = head;
    return head;
}

void free_memory(Node*& head) {
    if (head == nullptr) {
        return;
    }

    Node* current = head;
    do {
        Node* next_node = current->next; //????????? ????????? ?? ????. ????, -> ???? ????? ??? ? (*current).data
        delete current;
        current = next_node;
    } while (current != head);

    head = nullptr;
}


void add(Node*& head, int data) {
    Node* node = new Node();
    node->data = data;

    if (head == nullptr) {
        node->next = node;
        node->prev = node;
        head = node;
    }
    else {
        Node* current = head;
        while (current->next != head && current->next->data < data) {
            current = current->next;
        }
        if (current == head && current->data > data) {
            node->next = current;
            node->prev = current->prev;
            head = node;
        }
        else {
            node->next = current->next;
            node->prev = current;
        }
        node->next->prev = node;
        node->prev->next = node;
    }
}


void remove_before(Node*& head, int data) {
    Node* current = head;
    if (current != nullptr) {
        do {
            if (current->data == data) {
                Node* toDelete = current;
                current->prev->next = toDelete->next;
                toDelete->next->prev = current->prev; // ?????????? ??? ???????????? ????? toDelete = ?????????? ????? toDelete

                if (toDelete == head) {
                    head = toDelete->next;
                }

                current = toDelete->next;
                delete toDelete;
            }
            else {
                current = current->next;
            }
        } while (current != head);
    }
}

bool find(Node* head, int data) {
    Node* current = head;
    if (current != nullptr) {
        do {
            if (current->data == data) {
                return true;
            }
            current = current->next;
        } while (current != head);
    }
    return false;
}


void print(Node* head) {
    Node* current = head;
    if (current != nullptr) {
        do {
            std::cout << current->data << " ";
            current = current->next;
        } while (current != head);

        std::cout << std::endl;
    }
}


Node* symmetric_difference(Node* head1, Node* head2) {
    Node* result = nullptr;

    Node* current = head1;
    do {
        if (!find(head2, current->data)) {
            add(result, current->data);
        }
        current = current->next;
    } while (current != head1);

    current = head2;
    do {
        if (!find(head1, current->data)) {
            add(result, current->data);
        }
        current = current->next;
    } while (current != head2);

    return result;
}
