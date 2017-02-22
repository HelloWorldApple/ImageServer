#ifndef CHANNEL_H
#define CHANNEL_H
#include<functional>
#include<sys/epoll.h>
class Eventloop;
class Channel{

public:
    using EventCallback=std::function<void()>;
    //typedef std::function<void()> EventCallback;
    Channel(int fd,Eventloop* loop)
        :_fd(fd),_index(-1),_loop(loop),_events(0),_revents(0)
    {

    }

    ~Channel(){}
    void setReadCallback(const EventCallback &cb){_readcb=cb;}
    void setWriteCallback(const EventCallback &cb){_writecb=cb;}
    void setCloseCallback(const EventCallback &cb){_closecb=cb;}
    void setErrorCallback(const EventCallback &cb){_errorcb=cb;}

    void setInterestedInRead(bool on);
    void setInterestedInWrite(bool on);
    void update();
//use by epoll
    void set_revents(int insterested_events){_revents|=insterested_events;}
    void handleEvent();


    int getfd(){return _fd;}
    int events(){return _events;}
    Eventloop* getloop(){return _loop;}

    int set_index(int idx){_index=idx;}
    int get_index(){return _index;}
private:
    int _fd;
    int _index;
    Eventloop* _loop;

    int _events;
    int _revents;

    EventCallback _readcb,_writecb,_closecb,_errorcb;


};

#endif
