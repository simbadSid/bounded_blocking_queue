# TODO


## Objective:


## Build and test
This project uses the CMake framework.
In order to build the project, we need to run the following command
```
mkdir bin
cd bin
cmake ../
make;
```

Building the project results in generating the following executable files:
* __bin/main\<scenarioName\>__: TODO
* __bin/main\<scenarioName\>verbose__: TODO

In order to test the functional correctness of our algorithms, we have implemented a set of unit tests.
These tests may be run using:
```
cd bin
cmake ../
make;
ctes
```
The test framework that we used (CTest) is integrated within the CMake framework and requires no external installation.




# Description

Implement a thread-safe bounded blocking queue in C++ with the following operations:

1. **Constructor**: `BoundedBlockingQueue(int capacity)`
    - Initializes the queue with the given capacity.

2. **void Enqueue(int item)**:
    - Adds the given item to the queue.
    - If the queue is full, the calling thread should block until space becomes available.

3. **int Dequeue()**:
    - Removes and returns the front item from the queue.
    - If the queue is empty, the calling thread should block until an item becomes available.

4. **int Size()**:
    - Returns the number of elements currently in the queue.
    - This operation should be thread-safe.

## Requirements

- The queue uses condition variables and mutexes to achieve thread safety and to handle blocking behavior.
- Use the C++11 (or later) standard library.

## Example Usage

```cpp
#include <iostream>
#include <thread>
#include <vector>
#include "BoundedBlockingQueue.h"

void producer(BoundedBlockingQueue& queue) {
    for (int i = 0; i < 10; ++i) {
        queue.Enqueue(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(BoundedBlockingQueue& queue) {
    for (int i = 0; i < 10; ++i) {
        int item = queue.Dequeue();
        std::cout << "Consumed: " << item << std::endl;
    }
}

int main() {
    BoundedBlockingQueue queue(5);

    std::thread prodThread(producer, std::ref(queue));
    std::thread consThread(consumer, std::ref(queue));

    prodThread.join();
    consThread.join();

    return 0;
}
