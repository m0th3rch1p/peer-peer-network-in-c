#include "../lists/linkedList.h"

struct Queue {
    struct LinkedList* queue;

    void (*enqueue)(void* data, int dataLen, struct Queue* queue);
    void* (*dequeue)(struct Queue* queue);
};

struct Queue* queue_constructor ();
void queue_deconstructor (struct Queue* queue);