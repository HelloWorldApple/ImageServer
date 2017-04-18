#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include"Channel.h"
#include<functional>
#include<vector>
#include<unordered_map>
#include<memory>
#include<functional>
#include"Epoll.h"
#include"Mutex.h"
#include"Thread.h"
#include"WaitCondition.h"
using std::shared_ptr;
using std::vector;
using std::unordered_map;
using std::function;
class Eventloop{
public:
    using Func=std::function<void()>;
    explicit Eventloop(){}
    explicit Eventloop(const shared_ptr<Epoll>& epoller);
    void setEpoller(const shared_ptr<Epoll>& epoler){
        _epoller=epoler;
    }

    ~Eventloop();
    void loop();
    void quit();
    //void addin(const Func& func);
    //void wakeup();
    void updateChannel(Channel* c);
    void removeChannel(Channel* c);
    bool hasChannel(Channel* c);
private:

    vector<Channel*> _activeChannels;
    Channel* _curChannel;
    shared_ptr<Epoll> _epoller;
    bool _quit;
    unordered_map<int,Channel*> _channelMap;
};
class EventloopThread{

private:
    using Func=function<void(Eventloop*)>;

    Eventloop* _loop;
    Thread _thread;
    MutexLock _mutex;
    WaitCondition _cond;

    Func _callback;
public:
    explicit EventloopThread(const Func& callback=Func());
    ~EventloopThread();
    Eventloop* startLoopThread();
    void threadFunc();
};

#endif
