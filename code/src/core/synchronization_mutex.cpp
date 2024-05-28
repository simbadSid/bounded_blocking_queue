#include "synchronization_mutex.h"

#define CLASS_NAME "Mutex"

SynchronizationMutex::SynchronizationMutex()
{
    log("Creating a synchronization class: son \"%s\" class\n", CLASS_NAME);
}

SynchronizationMutex::~SynchronizationMutex()
{
    log("Destroying a synchronization class: son \"%s\" class\n", CLASS_NAME);
}

void SynchronizationMutex::enterCriticalSection()
{
    mtx.lock();
    log("Entering critical section tid=%d: son \"%s\" class\n", std::this_thread::get_id(), CLASS_NAME);
}

void SynchronizationMutex::leaveCriticalSection()
{
    log("Leaving critical section tid=%d: son \"%s\" class\n", std::this_thread::get_id(), CLASS_NAME);
    mtx.unlock();
    cond.notify_one();
}

void SynchronizationMutex::enterCriticalSectionWhenCondition(const std::function<bool()>& condition_to_enter)
{
    log("Entering critical section tid=%d start waiting for condition: son \"%s\" class\n",
        std::this_thread::get_id(), CLASS_NAME);

    std::unique_lock<std::mutex> unique_lock(mtx);
    cond.wait(unique_lock, condition_to_enter);

    log("Entering critical section tid=%d after completion of condition: son \"%s\" class\n",
        std::this_thread::get_id(), CLASS_NAME);
}
