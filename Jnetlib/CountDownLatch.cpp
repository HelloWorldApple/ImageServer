#include"CountDownLatch.h"
CountDownLatch::CountDownLatch(int count)
    :_lock(),_cond(_lock),_count(count)
{

}

int CountDownLatch::getCount(){
    MutexLockGuard guard(_lock);
    return _count;
}

void CountDownLatch::countDown(){
    MutexLockGuard guard(_lock);
    --_count;
    if(_count==0){
        _cond.wakeAll();
    }
}

void CountDownLatch::wait(){
    MutexLockGuard guard(_lock);
    while(_count){
        _cond.wait();
    }
}
