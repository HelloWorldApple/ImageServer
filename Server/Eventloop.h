#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include"Channel.h"
#include<functional>
#include<vector>
#include<unordered_map>
#include<memory>
#include"Epoll.h"
using std::shared_ptr;
using std::vector;
using std::unordered_map;
class Eventloop{
public:
    using Func=std::function<void()>;
    explicit Eventloop(const shared_ptr<Epoll>& epoller);
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

#endif
