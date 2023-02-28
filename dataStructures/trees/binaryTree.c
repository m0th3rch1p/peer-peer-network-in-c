#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binaryTree.h"

void insert_binary_tree(void* data, int dataLen, struct BinaryTree* tree);
void remove_binary_tree(void* data, struct BinaryTree* tree);
void* search_binary_tree(void* data, struct BinaryTree* tree);

struct BinaryTreeNode* binary_tree_node_constructor (void* data, int dataLen) {
    struct BinaryTreeNode* node = (struct BinaryTreeNode*) calloc(1, sizeof(struct BinaryTreeNode));
    if (!node) {
        fprintf(stderr, "error allocating memory for binary tree node\n");
        exit(9);
    }

    node->node = node_constructor(data, dataLen);
    node->left = NULL;
    node->right = NULL;
    return node;
};

void binary_tree_node_destructor(struct BinaryTreeNode *node) {
    node_deconstructor(node->node);
    free(node);
}

struct BinaryTree* binary_tree_contructor(int (*cmp)(void* data1, void* data2)) {
    struct BinaryTree* tree = (struct BinaryTree*)calloc(1, sizeof(struct BinaryTree));
    if (!tree) {
        fprintf(stderr, "error allocating memory for binary tree\n");
        return NULL;
    }

    tree->insert = insert_binary_tree;
    tree->remove = remove_binary_tree;
    tree->search = search_binary_tree;

    tree->cmp = cmp;
    return tree;
}

void destroy_tree (struct BinaryTreeNode* node) {
    if (node != NULL) {
        destroy_tree(node->left);
        destroy_tree(node->right);
        binary_tree_node_destructor(node);     
    }
}

void* print_tree (struct BinaryTreeNode* node) {
    if (node != NULL) {
        print_tree(node->left);
        print_tree(node->right);
        
        return node->node->data;     
    }
}


void binary_tree_destructor(struct BinaryTree* tree) {
    destroy_tree(tree->root);
}

struct BinaryTreeNode* iterate_tree (void* data, struct BinaryTreeNode* cursor, int* cmp, struct BinaryTree* tree) {    
    *cmp = tree->cmp(data, cursor->node->data);
    if (*cmp == -1 && cursor->left) cursor = cursor->left;
    else if (*cmp == 1 && cursor->right) cursor = cursor->right;
    else return cursor;

    iterate_tree(data, cursor, cmp, tree);
}

struct BinaryTreeNode* inOrderSuccessor (struct BinaryTreeNode* root, struct BinaryTree* tree) {
    while (root->left) {
        root = root->left;
    }
    return root;
}


void insert_binary_tree(void* data, int dataLen, struct BinaryTree* tree) {
    if (!tree->root) {
        tree->root = binary_tree_node_constructor(data, dataLen);
        return;
    }

    int direction = -2;
    struct BinaryTreeNode* cursor = iterate_tree(data, tree->root, &direction, tree);
    if (direction == 0) {
        fprintf(stderr, "[-] item already exists\n");
    } else if (direction == 1) {
        cursor->right = binary_tree_node_constructor(data, dataLen);
    } else if (direction == -1) {
        cursor->left = binary_tree_node_constructor(data, dataLen);
    }
}

void remove_binary_tree(void* data, struct BinaryTree* tree) {
    if(!tree->root) {
        fprintf(stderr, "tree is empty\n");
        return;
    }

    int direction;
    struct BinaryTreeNode* node = iterate_tree(data, tree->root, &direction, tree);
    if (direction != 0) {
        fprintf(stderr, "item not found in tree.\n");
        return;
    }

    struct BinaryTreeNode * node_to_remove;
    if (node->left == NULL && node->right == NULL) { 
        node_to_remove = node;
    } else if (node->right == NULL) {
        struct BinaryTreeNode* tmp = node;
        node = node->left;

        node_to_remove = tmp;
    } else if (node->left == NULL) {
        struct BinaryTreeNode* tmp = node;
        node = node->right;

        node_to_remove = tmp;
    } else {
        struct BinaryTreeNode* minNode = inOrderSuccessor(node->right, tree);
        node_overwrite_data(minNode->node->data, minNode->node->dataLen, node->node);

        node_to_remove = minNode;
    }
    binary_tree_node_destructor(node_to_remove);
}

void* search_binary_tree(void* data, struct BinaryTree* tree) {
    int direction;
    struct BinaryTreeNode* node = iterate_tree(data, tree->root, &direction, tree);

    if(direction == 0) return node->node->data;
    else return NULL;
}