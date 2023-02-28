#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"

struct Node* node_constructor (void* data, int dataLen) {
    struct Node* node = calloc(1, sizeof(struct Node));
    if(!node) {
        fprintf(stderr, "error allocating memory. (node)\n");
        exit(9);
    }

    node->data = calloc(1, dataLen);
    memcpy(node->data, data, dataLen);
    
    node->dataLen = dataLen;
    return node;
}

void node_deconstructor (struct Node* node) {
    free(node->data);
    free(node);
}

void node_overwrite_data (void* data, int dataLen, struct Node* node) {
    //Clean up node data and resize the node
    memset(node->data, 0, node->dataLen);

    node->data = realloc(node->data, dataLen);
    if(node->data == NULL) {
        fprintf(stderr, "error resizing node size\n");
        exit(9);
    }

    memcpy(node->data, data, dataLen);
    node->dataLen = dataLen;
}