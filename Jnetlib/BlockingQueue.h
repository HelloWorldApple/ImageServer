#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H
#include"Mutex.h"
#include"WaitCondition.h"
#include<deque>

template<class T>
class BlockingQueue
{
public:

    BlockingQueue():m_mutex(),m_notEmpty(m_mutex)
    {

    }


    ~BlockingQueue()
    {

    }


    void push(const T& item)
    {
        MutexLockGuard lockguard(m_mutex);
        m_deque.push_back(item);
        m_notEmpty.wakeOne();
    }


    T take()
    {
        MutexLockGuard lockguard(m_mutex);
        //can not replace 'while' with 'if',because 'if' will
        //cause spurious wakeup.
        while(m_deque.empty()){
            m_notEmpty.wait();
        }
        T res(m_deque.front());
        m_deque.pop_front();
        return res;
    }

    size_t size()//dont add const,because m_mutex should be changed
    {
        MutexLockGuard lockguard(m_mutex);
        return m_deque.size();
    }
private:

    MutexLock m_mutex;

    WaitCondition m_notEmpty;

    std::deque<T> m_deque;
};




#endif
