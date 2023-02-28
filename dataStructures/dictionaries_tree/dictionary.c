#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

void insert_dict_tree(void* key, int keyLen, void* val, int valLen, struct Dictionary* dict);
void remove_dict_tree(void* key, struct Dictionary* dict);
struct DictionaryNode* search_dict_tree(void* key, int keyLen, struct Dictionary* dict);

struct DictionaryNode dictionary_node_constructor(void* key, int keyLen, void* val, int valLen) {
    struct DictionaryNode node = { 0 };
    

    node.key = calloc(1, keyLen);
    if(!node.key) {
        fprintf(stderr, "error allocating memory for dictionary node key\n");
        exit(9);
    }
    memcpy(node.key, key, keyLen);
    node.keyLen = keyLen;

    node.val = calloc(1, valLen);
    if (!node.val) {
        fprintf(stderr, "error allocating memory for dictionary node value\n");
        free(node.key);
        // free(node);
        exit(9);
    }
    memcpy(node.val, val, valLen);
    node.valdataLen = valLen;

    return node;
}

struct Dictionary* dictionary_constructor (int (*cmp)(void* data1, void* data2)) {
    struct Dictionary* dictionary = (struct Dictionary*)calloc(1, sizeof(struct Dictionary));
    if (!dictionary) {
        fprintf(stderr, "error allocating memory for dictionary\n");
        exit(9);
    }
    dictionary->dict = binary_tree_contructor(cmp);

    dictionary->insert = insert_dict_tree;
    dictionary->remove = remove_dict_tree;
    dictionary->search = search_dict_tree;

    return dictionary;
}

void dictionary_destructor(struct Dictionary* dict) {
    binary_tree_destructor(dict->dict);
    free(dict);
}

void insert_dict_tree(void* key, int keyLen, void* val, int valLen, struct Dictionary* dict) {
    struct DictionaryNode node = dictionary_node_constructor(key, keyLen, val, valLen);

    dict->dict->insert((void *)&node, sizeof(node), dict->dict);
}

void remove_dict_tree(void* key, struct Dictionary* dict) {
    dict->dict->remove(key, dict->dict);
}

struct DictionaryNode* search_dict_tree(void* key, int keyLen, struct Dictionary* dict) {
    struct DictionaryNode node = { 0 };
    node.key = key;
    node.keyLen = keyLen;

    node.valdataLen = 0;

    void* data = dict->dict->search(&node, dict->dict);

    return (struct DictionaryNode*)data;
}

int str_cmp (void* data1, void* data2) {
    int cmp = strcmp((char *)data1, (char*)data2);

    if (cmp < 0) return -1;
    else if (cmp > 0) return 1;
    return cmp;
}

int str_dict_cmp (void* data1, void* data2) {
    return str_cmp((char*)((struct DictionaryNode*)data1)->key, (char*)((struct DictionaryNode*)data2)->key);
}