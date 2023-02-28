#include "../node/node.h"

struct LinkedListNode {
    struct Node* node;
    struct LinkedListNode* next;
};

struct LinkedList {
    struct LinkedListNode* head;
    int length;

    void (*insert)(int index, void* data, int dataLen, struct LinkedList* list);
    void (*remove)(int index, struct LinkedList* list);
    void* (*search)(int index, struct LinkedList* list);
};

struct LinkedList* linked_list_constructor ();
void linked_list_deconstructor(struct LinkedList* list);