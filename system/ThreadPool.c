#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ThreadPool.h"

void* thread_job (void* arg);

void add_work (struct ThreadJob job, struct ThreadPool* thread_pool);

struct ThreadPool* thread_pool_constructor(int num_threads) {
    struct ThreadPool* thread_pool = (struct ThreadPool*)calloc(1, sizeof(struct ThreadPool));
    if (!thread_pool) {
        perror("error allocating thread pool....");
        exit(1);
    }

    thread_pool->num_threads = num_threads;
    thread_pool->active = 1;
    thread_pool->work = queue_constructor();
    thread_pool->add_work = add_work;

    thread_pool->pool = (pthread_t *)calloc(num_threads, sizeof(pthread_t));
    if (!thread_pool->pool) {
        perror("error allocating thread pool -> pool....");
        free(thread_pool);
        exit(1);
    }

    // Initialize mutex
    thread_pool->lock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;

    // Initialize signal
    thread_pool->signal = (pthread_cond_t) PTHREAD_COND_INITIALIZER;

    // Initialize the thread pool
    printf("creating thread pool\n");
    for (int i = 0; i < num_threads; i++)
    {
        pthread_create(&thread_pool->pool[i], NULL, thread_job, (void *)thread_pool);
    }
    printf("done creating thread pool\n");

    return thread_pool;
}

void thread_pool_deconstructor(struct ThreadPool* thread_pool) {
    thread_pool->active = 0;
    for (int i = 0; i < thread_pool->num_threads; i++)
    {
        pthread_cond_signal(&thread_pool->signal);
    }
    
    for (int i = 0; i < thread_pool->num_threads; i++)
    {
        pthread_join(thread_pool->pool[i], NULL);
    }
    
    queue_deconstructor(thread_pool->work);
    free(thread_pool->pool);
    free(thread_pool);
}

struct ThreadJob thread_job_constructor(void* (*job)(void* arg), void* arg) {    
    struct ThreadJob thread_job;
    thread_job.job = job;
    thread_job.arg = arg;
    return thread_job;
}


void add_work (struct ThreadJob job, struct ThreadPool* thread_pool) {
    pthread_mutex_lock(&thread_pool->lock);
    thread_pool->work->enqueue(&job, sizeof(job), thread_pool->work);
    // thread_job_deconstructor(job);
    pthread_cond_signal(&thread_pool->signal);
    pthread_mutex_unlock(&thread_pool->lock);
    // pthread_cond_signal(&thread_pool->signal);
}

void* thread_job (void* arg) {
    struct ThreadPool* thread_pool = (struct ThreadPool*)arg;
    while (thread_pool->active == 1) {
        if (pthread_mutex_lock(&thread_pool->lock) == 0) {
            pthread_cond_wait(&thread_pool->signal, &thread_pool->lock);
            struct ThreadJob* pJob = ((struct ThreadJob *)thread_pool->work->dequeue(thread_pool->work));
            struct ThreadJob job = { 0 };
            if(pJob) {
                job = *pJob;
                thread_pool->work->pop(thread_pool->work);
                printf("Popping item: %d\n", thread_pool->work->queue->length);
            }

            pthread_mutex_unlock(&thread_pool->lock);
            if (job.job != NULL) {
                printf("Executing job...\n");
                job.job(job.arg);
                // thread_job_deconstructor(job);
            }
        }
    }
    return NULL;
}
