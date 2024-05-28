#include <iostream>
#include <thread>
#include <vector>

#include "queue_thread_safe.h"


#define DATA_TYPE int


void producer(BoundedBlockingQueue<DATA_TYPE>* queue) {
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "Produced: " << i << std::endl;
        queue->enqueue(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(BoundedBlockingQueue<DATA_TYPE>* queue) {
    for (int i = 0; i < 10; ++i) {
        DATA_TYPE item = queue->dequeue();
        std::cout << "Consumed: " << item << std::endl;
    }
}

/*
void test()
{
    BoundedBlockingQueue<DATA_TYPE> queue;
    queue.enqueue(33);
    queue.dequeue();
}
 */

int main()
{
    std::cout << "Begin RUN!" << std::endl;

    //test();
    /*
     */
    auto *queue = new BoundedBlockingQueue<DATA_TYPE>();

    std::thread prodThread(producer, std::ref(queue));
    std::thread consThread(consumer, std::ref(queue));

    prodThread.join();
    consThread.join();

    delete queue;

    std::cout << "End RUN!" << std::endl;
    return 0;
}
