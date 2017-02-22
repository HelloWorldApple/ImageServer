#ifndef EPOLL_H
#define EPOLL_H
#include<sys/epoll.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<vector>
using std::vector;
class Epoll{
public:
    explicit Epoll(int maxfds=0,bool ET=false);
    ~Epoll();
    void add(int fd,uint32_t event);
    void del(int fd,uint32_t event);
    void mod(int fd,uint32_t event);
    int wait(int timeout);
    const struct epoll_event* getEvents(){
        return _events;
    }

private:
    int _maxfds;
    bool _et;
    int _epfd;
    struct epoll_event* _events;

};

#endif
