#ifndef SYNCHRONIZATION_MUTEX_H
#define SYNCHRONIZATION_MUTEX_H

#include <mutex>
#include <condition_variable>
#include <functional>

#include "synchronization.h"

class SynchronizationMutex : public Synchronization
{
private:
    std::mutex mtx;
    std::condition_variable cond;

public:
    explicit SynchronizationMutex();
    ~SynchronizationMutex() override;

    void enterCriticalSection() override;
    void leaveCriticalSection() override;
    void enterCriticalSectionWhenCondition(const std::function<bool()>& condition_to_enter) override;
};

#endif // SYNCHRONIZATION_MUTEX_H
