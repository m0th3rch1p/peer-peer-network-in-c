#ifndef THREAD_POOL_DEF
#define THREAD_POOL_DEF

#include <pthread.h>

#include "../dataStructures/queues/queue.h"

struct ThreadJob {
    void* (*job)(void* arg);
    void* arg;
};

struct ThreadPool {
    int num_threads;
    int active;

    struct Queue* work;
    pthread_t *pool;
    pthread_mutex_t lock;
    pthread_cond_t signal;

    void (*add_work)(struct ThreadJob job, struct ThreadPool* thread_pool);
};

struct ThreadPool* thread_pool_constructor(int num_threads);
void thread_pool_deconstructor(struct ThreadPool* thread_pool);

struct ThreadJob thread_job_constructor(void* (*job)(void *arg), void* arg);
#endif