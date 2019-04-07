#pragma once
#include "Common.h"

class IMultiThread
{
public:
    void WaitWhenCompleted();
    virtual ~IMultiThread() {}
protected:
    virtual void StartThread(void * context) = 0;
protected:
    std::vector<std::thread> m_threads;
};

