#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedList.h"

void insert_linked_list (int index, void* data, int dataLen, struct LinkedList* list);
void remove_linked_list (int index, struct LinkedList* list);
void* search_linked_list (int index, struct LinkedList* list);


struct LinkedList* linked_list_constructor () {
    struct LinkedList* list = (struct LinkedList*)calloc(1, sizeof(struct LinkedList));
    if (!list) {
        fprintf(stderr, "error allocating memory (linkedlist)\n");
        exit(9);
    }

    list->insert = insert_linked_list;
    list->remove = remove_linked_list;
    list->search = search_linked_list;

    return list;
}

void linked_list_deconstructor(struct LinkedList* list) {
    struct LinkedListNode* cursor = list->head;
    while (cursor) {
        struct LinkedListNode* tmp = cursor;
        cursor = cursor->next;

        node_deconstructor(tmp->node);
        free(tmp);
    }
}

struct LinkedListNode* linked_list_node_constructor (void * data, int data_len) {
    struct LinkedListNode* listNode = (struct LinkedListNode*) calloc(1, sizeof(struct LinkedListNode));
    if (!listNode) {
        fprintf(stderr, "error allocating memory for linked list node\n");
        exit(9);
    }

    listNode->node = node_constructor (data, data_len);
    return listNode;
}

struct LinkedListNode* iterate_list (int index, struct LinkedList* list) {
    if (index > list->length) {
        fprintf(stderr, "index is out of bounds\n");
        return NULL;
    }

    int i = 0;
    struct LinkedListNode* cursor = list->head;
    while (cursor) {
        if (i > index) break;
        else if (i == index) return cursor;

        cursor = cursor->next;
        i++;
    }
    return NULL;
}

void insert_linked_list (int index, void* data, int dataLen, struct LinkedList* list) {
    struct LinkedListNode* node =  NULL;
    if (index == 0) {
        node = linked_list_node_constructor(data, dataLen);
        if (list->head == NULL) {
            list->head = node;
        } else {
            node->next = list->head;
            list->head = node;
        }
        return;
    }

    struct LinkedListNode *cursor = iterate_list(index - 1, list);
    if (cursor == NULL) {
        fprintf(stderr, "Error inserting node\n");
        return;
    }

    list->length++;
    node = linked_list_node_constructor(data, dataLen);
    cursor->next = node;
}

void remove_linked_list (int index, struct LinkedList* list) {
    struct LinkedListNode * node_to_remove;
    if (index == 0) {
        node_to_remove = list->head;
        list->head = node_to_remove->next;

    } else {
        struct LinkedListNode *cursor = iterate_list(index - 1, list);
        if (cursor == NULL) {
            fprintf(stderr, "Error inserting node\n");
            return;
        }
    
        node_to_remove = cursor->next;
        cursor->next = node_to_remove->next;
    }

    node_deconstructor(node_to_remove->node);
    free(node_to_remove);

    list->length--;
}

void* search_linked_list (int index, struct LinkedList* list) {
    struct LinkedListNode *cursor = iterate_list(index, list);
    if (cursor == NULL) {
        fprintf(stderr, "Error inserting node\n");
        return NULL;
    }

    return cursor->node->data;
}
