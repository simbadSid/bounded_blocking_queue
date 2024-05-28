#ifndef TEST_SHARED_ROUTINE_H
#define TEST_SHARED_ROUTINE_H

#include <thread>
#include <random>
#include <iostream>

#include "queue_thread_safe.h"


#define DATA_TYPE       int

// Seed for random number generation
std::random_device rd;
std::mt19937 gen(rd());


DATA_TYPE generate_data()
{
    //TODO
    std::uniform_int_distribution<> dis(0, 10000);
    return dis(gen);
}

bool check_correctness(BoundedBlockingQueue<DATA_TYPE> *queue)
{
    return (queue->size() == 0);
}


int test_creation_destruction()
{
    std::cout << "Begin RUN!" << std::endl;

    auto queue = new BoundedBlockingQueue<DATA_TYPE>();
    delete queue;
    std::cout << "End RUN!" << std::endl;

    return 0;
}

int test_N_enqueue_N_dequeue(const unsigned &n_enqueue)
{
    std::cout << "Begin RUN!" << std::endl;

    auto queue = new BoundedBlockingQueue<DATA_TYPE>();
    std::vector<DATA_TYPE> allData = std::vector<DATA_TYPE>(n_enqueue);

    // Enqueue all
    for (unsigned i=0; i<n_enqueue; ++i)
    {
        DATA_TYPE d = generate_data();
        queue->enqueue(d);
        allData[i] = d;
    }

    // Dequeue all
    bool is_correct = true;
    for (unsigned i=0; i<n_enqueue; ++i)
    {
        DATA_TYPE d = queue->dequeue();
        is_correct = (is_correct & (d==allData[i]));
        if (not is_correct)
            break;
    }

    delete queue;
    std::cout << "End RUN!" << std::endl;

    if (is_correct)
        return 0;
    else
        return -1;
}

int test_N_enqueue_dequeue_rnd_order(const unsigned &n_enqueue)
{
    std::cout << "Begin RUN!" << std::endl;

    auto queue = new BoundedBlockingQueue<DATA_TYPE>();

    std::uniform_int_distribution<> dis(0, 1);
    unsigned n_remainingEnqueue = n_enqueue;
    unsigned n_remainingDequeue = n_enqueue;

    while (true)
    {
        if ((n_remainingEnqueue > 0) && (n_remainingDequeue > 0))
        {
            int bit = dis(gen);
            if (bit or (queue->size() == 0))
            {
                queue->enqueue(generate_data());
                -- n_remainingEnqueue;
            }
            else if (n_remainingDequeue < n_remainingEnqueue)
            {
                queue->dequeue();
                -- n_remainingDequeue;
            }
        }
        else if (n_remainingEnqueue > 0)
        {
            queue->enqueue(generate_data());
            -- n_remainingEnqueue;
        }
        else if (n_remainingDequeue > 0)
        {
            queue->dequeue();
            -- n_remainingDequeue;
        }
        else
            break;
    }
    bool test = check_correctness(queue);
    delete queue;
    std::cout << "End RUN!" << std::endl;

    if (test)
        return 0;
    else
        return -1;
}

void producer(BoundedBlockingQueue<DATA_TYPE>* queue) {
    for (int i = 0; i < 10; ++i)
    {
        DATA_TYPE x = generate_data();
        std::cout << "Produced: " << x << std::endl;
        queue->enqueue(x);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(BoundedBlockingQueue<DATA_TYPE>* queue) {
    for (int i = 0; i < 10; ++i) {
        DATA_TYPE item = queue->dequeue();
        std::cout << "Consumed: " << item << std::endl;
    }
}

int test_parallel_N_producer_N_consumer(const unsigned &n_producer, const unsigned &n_consumer)
{
    std::cout << "Begin RUN!" << std::endl;

    auto *queue = new BoundedBlockingQueue<DATA_TYPE>();
    std::vector<std::thread> vector_prodThread      (n_producer);
    std::vector<std::thread> vector_consumerThread  (n_consumer);

    vector_prodThread       .reserve(n_producer);
    vector_consumerThread   .reserve(n_producer);

    // Create and launch threads
    for (unsigned i=0; i<n_producer; i++)
        vector_prodThread[i] = std::thread(producer, std::ref(queue));
    for (unsigned i=0; i<n_consumer; i++)
        vector_consumerThread[i] = std::thread(consumer, std::ref(queue));

    // Wait for threads to finish and wipe out
    for (unsigned i=0; i<n_producer; i++)
        vector_prodThread[i].join();
    for (unsigned i=0; i<n_consumer; i++)
        vector_consumerThread[i].join();

    bool test = check_correctness(queue);
    delete queue;
    std::cout << "End RUN!" << std::endl;

    if (test)
        return 0;
    else
        return -1;
}

#endif //TEST_SHARED_ROUTINE_H
