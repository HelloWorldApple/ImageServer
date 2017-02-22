#ifndef BOARDEDBLOCKINGQUEUE_H
#define BOARDEDBLOCKINGQUEUE_H

#include"Mutex.h"
#include"WaitCondition.h"
#include<vector>


#define DEFAULTSIZE 20

template<class T>
class BoardedBlockingQueue
{
public:
    BoardedBlockingQueue(int size = DEFAULTSIZE)
        :m_mutex(),m_notEmpty(m_mutex),
          m_notFull(m_mutex),in(0),out(0),
          m_size(size),
          queue(m_size)
    {

    }

    ~BoardedBlockingQueue()
    {

    }

    void put(const T& item)
    {
        MutexLockGuard lockguard(m_mutex);
        while((in+1)%m_size == out)
        {
            m_notFull.wait();
        }
        queue[in]=item;
        in=(in+1)%m_size;
        m_notEmpty.wakeOne();
    }

    T take()
    {
        MutexLockGuard lockguard(m_mutex);
        while(in==out)
        {
            m_notEmpty.wait();
        }
        T res(queue[out]);
        out = (out+1)%m_size;
        m_notFull.wakeOne();

        return res;
    }

private:
    MutexLock m_mutex;

    WaitCondition m_notFull;
    WaitCondition m_notEmpty;

    int in;//first index can push
    int out;//first index can take

    size_t m_size;
    std::vector<T> queue;
};


#endif
