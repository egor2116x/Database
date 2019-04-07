#include "IMultiThread.h"

void IMultiThread::WaitWhenCompleted()
{
    for (auto & tr : m_threads)
    {
        tr.join();
    }
}
