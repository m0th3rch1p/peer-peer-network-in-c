#include "../trees/binaryTree.h"

#ifndef DICTIONARY_DEF
#define DICTIONARY_DEF
struct DictionaryNode {
    void* key;
    int keyLen;

    void* val;
    int valdataLen;
};

struct Dictionary {
    struct BinaryTree* dict;

    void (*insert)(void* key, int keyLen, void* val, int valLen, struct Dictionary* dict);
    void (*remove)(void* key, struct Dictionary* dict);
    struct DictionaryNode* (*search)(void* key, int keyLen, struct Dictionary* dict);
};

struct Dictionary* dictionary_constructor (int (*cmp)(void* data1, void* data2));
void dictionary_destructor(struct Dictionary* dict);

// Common comparison functions
int str_dict_cmp (void* data1, void* data2);

#endif

