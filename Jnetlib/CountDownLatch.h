#ifndef COUNTDOWNLATCH_H
#define COUNTDOWNLATCH_H
#include<Mutex.h>
#include<WaitCondition.h>
class CountDownLatch{
private:
    MutexLock _lock;
    WaitCondition _cond;
    int _count;
public:
    explicit CountDownLatch(int count);
    int getCount();
    void countDown();
    void wait();
};

#endif
