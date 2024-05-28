#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H

#include <cstdarg>
#include <functional>

#include "tools.h"

class Synchronization
{
public:
    Synchronization()
    {
        log("Creating a synchronization class: father class\n");
    }
    virtual ~Synchronization()
    {
        log("Destroying a synchronization class: father class\n");
    }

    virtual void enterCriticalSection() = 0;
    virtual void leaveCriticalSection() = 0;
    virtual void enterCriticalSectionWhenCondition(const std::function<bool()>& condition) = 0;
};

#endif //SYNCHRONIZATION_H
