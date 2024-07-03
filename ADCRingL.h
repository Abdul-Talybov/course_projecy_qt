#pragma once
struct Node {
    int data;
    Node* next;
    Node* prev;
};

Node* init(int data);
void free_memory(Node*& head);
void add(Node*& head, int data);
void remove_before(Node*& head, int data);
bool find(Node* head, int data);
void print(Node* head);
Node* symmetric_difference(Node* head1, Node* head2);
