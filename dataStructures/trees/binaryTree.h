#include "../node/node.h"

#ifndef BINARY_TREE_DEF
#define BINARY_TREE_DEF

struct BinaryTreeNode {
    struct Node* node;
    struct BinaryTreeNode* left;
    struct BinaryTreeNode* right;
};

struct BinaryTree {
    struct BinaryTreeNode* root;

    int (*cmp)(void* data1, void* data2);

    void (*insert)(void* data, int dataLen, struct BinaryTree* tree);
    void (*remove)(void* data, struct BinaryTree* tree);
    void* (*search)(void* data, struct BinaryTree* tree);
};
#endif

struct BinaryTree* binary_tree_contructor(int (*cmp)(void* data1, void* data2));
void binary_tree_destructor(struct BinaryTree* tree);

void* print_tree (struct BinaryTreeNode* node);
