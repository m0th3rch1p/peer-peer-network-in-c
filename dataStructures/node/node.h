#ifndef NODE_DEF
#define NODE_DEF

struct Node {
    void* data;
    int dataLen;
};

struct Node* node_constructor (void* data, int dataLen);
void node_deconstructor (struct Node* node);
void node_overwrite_data (void* data, int dataLen, struct Node* node);

#endif