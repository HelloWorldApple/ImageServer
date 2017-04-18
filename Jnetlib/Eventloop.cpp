#include"Eventloop.h"
#include<iostream>
const int WAITTIME=10000;
const int NEW=-1;
const int ADDED=1;
const int DELED=2;
Eventloop::Eventloop(const shared_ptr<Epoll> &epoller):
_curChannel(nullptr),_epoller(epoller),_quit(false)
{

}

Eventloop::~Eventloop(){

}

void Eventloop::loop(){
    while(!_quit){
        //std::cout<<"inter loop"<<std::endl;
        _activeChannels.clear();
        int eventnum=_epoller->wait(WAITTIME);
        const struct epoll_event *events=_epoller->getEvents();
        //_activeChannels.reserve(eventnum);
        for(int i=0;i<eventnum;i++){
            _curChannel=_channelMap[events->data.fd];
            _curChannel->set_revents(events->events);
            //_activeChannels.push_back(_curChannel);
            //std::cout<<"handle"<<std::endl;
            _curChannel->handleEvent();
        }
    }
}

void Eventloop::quit(){

    _quit=true;
}
/*
void Eventloop::addin(const Func& func){

}

void Eventloop::wakeup(){

}
*/
void Eventloop::updateChannel(Channel* c){

    int fd=c->getfd();
    int index=c->get_index();
    if(index==NEW||index==DELED){
        if(index==NEW){
            _channelMap[fd]=c;//add the channel to map
        }else{

        }
        c->set_index(ADDED);
        _epoller->add(fd,c->events());
    }else{
        if(c->events()==0){//interested events is none,delete the fd in epoll
            _epoller->del(fd,0);
            c->set_index(DELED);
        }
        else {
            _epoller->mod(fd,c->events());
        }
    }

}

void Eventloop::removeChannel(Channel* c){

    _channelMap.erase(c->getfd());
    if(c->get_index()==ADDED){
        _epoller->del(c->getfd(),0);
    }
    c->set_index(NEW);
}

bool Eventloop::hasChannel(Channel* c){
    return _channelMap.find(c->getfd())!=_channelMap.end();
}

/**********eventloopthread******************/
EventloopThread::EventloopThread(const Func& callback)
    :
      _thread(std::bind(&EventloopThread::threadFunc,this)),
      _loop(0),
      _mutex(),
      _cond(_mutex),
      _callback(callback)

{

}

Eventloop* EventloopThread::startLoopThread(){
    _thread.run();
    {
        MutexLockGuard lock(_mutex);
        while(_loop==nullptr){
            _cond.wait();
        }
    }
    return _loop;
}

void EventloopThread::threadFunc(){
    Eventloop loop;
    if(_callback){
        _callback(&loop);
    }
    {
        MutexLockGuard lock(_mutex);
        _loop=&loop;
        _cond.wakeOne();
    }
    loop.loop();
    _loop=nullptr;
}




