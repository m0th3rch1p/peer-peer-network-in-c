#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

void enqueue (void* data, int dataLen, struct Queue* queue);
void* dequeue (struct Queue* queue);

struct Queue* queue_constructor () {
    struct Queue* queue = (struct Queue*) calloc(1, sizeof(struct Queue));
    if(!queue) {
        fprintf(stderr, "error allocating queue memory\n");
        exit(9);
    }

    queue->queue = linked_list_constructor();
    queue->enqueue = enqueue;
    queue->dequeue = dequeue;
    return queue;
}

void queue_deconstructor (struct Queue* queue) {
    linked_list_deconstructor(queue->queue);
    free(queue);
}

void enqueue (void* data, int dataLen, struct Queue* queue) {
    queue->queue->insert(queue->queue->length, data, dataLen, queue->queue);
}

void* dequeue (struct Queue* queue) {
    void* data = queue->queue->search(0, queue->queue);
    queue->queue->remove(0, queue->queue);
    return data;
}
