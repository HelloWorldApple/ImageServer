
#ifndef THREADPOOL_H
#define THREADPOOL_H
#include<pthread.h>
#include<unistd.h>
#include<deque>
#include<vector>
#include<iostream>
#include<errno.h>
#include<string.h>
#include"Mutex.h"
#include"Task.h"
#include"WaitCondition.h"
//#include"Global.h"
#include"Thread.h"
#define DEFAULT_POOL_SIZE 10

const int STARTED=0;
const int STOPPED=1;
class ThreadPool
{
public:

    ThreadPool(int pool_size=DEFAULT_POOL_SIZE);
    ~ThreadPool();

    int destroy();

    void workerThread();
    int addTask(const Task&);
    size_t dequeSize();
private:
    int init();

private:
    int m_pool_size;
    MutexLock m_task_mutex;
    WaitCondition m_task_cond_var;

    std::vector<Thread> m_ThreadVec;
    std::deque<Task> m_tasks;
    volatile int m_pool_state;


};
#endif

/*//simple version
#ifndef THREADPOOL_H
#define THREADPOOL_H
#include<pthread.h>
#include<unistd.h>
#include<deque>
#include<vector>
#include<iostream>
#include<errno.h>
#include<string.h>
#include"Mutex.h"
#include"Task.h"
#include"WaitCondition.h"
#include"BlockingQueue.h"
//#include"Global.h"
#include"Thread.h"
#define DEFAULT_POOL_SIZE 10

const int STARTED=0;
const int STOPPED=1;
class ThreadPool
{
public:

    ThreadPool(int pool_size=DEFAULT_POOL_SIZE);
    ~ThreadPool();

    int destroy();

    void workerThread();
    int addTask(const Task&);
    size_t dequeSize();
private:
    int init();

private:
    int m_pool_size;
    MutexLock m_task_mutex;
    WaitCondition m_task_cond_var;

    std::vector<Thread> m_ThreadVec;
    BlockingQueue<Task> m_tasks;
    volatile int m_pool_state;


};
#endif
*/
