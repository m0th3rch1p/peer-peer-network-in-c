#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./dataStructures/trees/binaryTree.h"

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


int main () {
    struct BinaryTree *tree = binary_tree_contructor(str_cmp);

    char key_arr[1000000][5] = {0};    
    for (int i = 0; i < 1000000; i++) {
        char key[5] = {0};
        char val[30] = {0};

        rand_string(key, 5);
        rand_string(val, 30);

        tree->insert((void*)key, strlen(key), tree);
        strcpy(key_arr[i], key);
    }

    for (int i = 0; i < 1000000; i++) {
        void* data = tree->search((void*)key_arr[i], tree);
        if (data) printf("%s\n", (char *)data);
        else printf("%s not found\n", key_arr[i]);
    }

    binary_tree_destructor(tree);
}