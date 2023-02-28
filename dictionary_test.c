#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./dataStructures/dictionaries_tree/dictionary.h"

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
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

int main () {
    struct Dictionary* dictionary = dictionary_constructor(str_dict_cmp);
    char key_arr[1000000][5] = {0};
    for (int i = 0; i < 1000000; i++) {
        char key[5] = {0};
        char val[30] = {0};

        rand_string(key, 5);
        rand_string(val, 30);

        dictionary->insert((void*)key, sizeof(char[strlen(key)]), (void*)val, sizeof(char[strlen(val)]), dictionary);
        strcpy(key_arr[i], key);
    }   

    for (int i = 0; i < 1000000; i++) {
        struct DictionaryNode* node = dictionary->search(key_arr[i], sizeof(char[strlen(key_arr[i])]), dictionary);
        if (node) printf("%s\n", (char*)node->val);
        else printf("%s\n", key_arr[i]);
    }

    dictionary_destructor(dictionary);
}