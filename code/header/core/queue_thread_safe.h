#ifndef BOUNDED_BLOCKING_QUEUE_QUEUE_THREAD_SAFE_H
#define BOUNDED_BLOCKING_QUEUE_QUEUE_THREAD_SAFE_H

#include <iostream>
#include <queue>
#include <functional>

#include "tools.h"
#include "synchronization_mutex.h"


/*
 *  Mutex: Use std::mutex to protect shared data.
    Condition Variables: Use std::condition_variable to wait for and notify about changes in the queue state.
    Blocking: Ensure that Enqueue blocks when the queue is full and Dequeue blocks when the queue is empty.
 */

#define CLASS_NAME "BoundedBlockingQueue"

template <typename T>
class BoundedBlockingQueue
{
private:
    std::queue<T>       *queue;
    Synchronization     *synchronization;

public:
    explicit BoundedBlockingQueue()
    {
        log("Creating %s\n", CLASS_NAME);
        this->queue             = new std::queue<T>();
        this->synchronization   = new SynchronizationMutex();
    }

    ~BoundedBlockingQueue()
    {
        delete this->queue;
        delete this->synchronization;
        log("Destroying %s\n", CLASS_NAME);
    }

    void enqueue(T item)
    {
        log("Enqueuing an element\n");
        this->synchronization->enterCriticalSection();
        this->queue->push(item);
        this->synchronization->leaveCriticalSection();
    }


    T dequeue()
    {
        log("De-queuing an element\n");

        this->synchronization->enterCriticalSectionWhenCondition([this]() { return !this->is_empty_no_thread_safe(); });

        T res = this->queue->front();
        this->queue->pop();
        this->synchronization->leaveCriticalSection();
        return res;
    }

    size_t size()
    {
        log("Accessing queue size\n");
        this->synchronization->enterCriticalSection();
        size_t res = this->queue->size();
        this->synchronization->leaveCriticalSection();
        return res;
    }

    bool is_empty_no_thread_safe()
    {
        log("Accessing queue size (not thread safe)\n");
        return this->queue->empty();
    }


    /*
    explicit BoundedBlockingQueue(unsigned int nb_elem);
    ~BoundedBlockingQueue();

    void            enqueue(T item);
    T               dequeue();
    unsigned int    size();
    */
};

#endif //BOUNDED_BLOCKING_QUEUE_QUEUE_THREAD_SAFE_H
