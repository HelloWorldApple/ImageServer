#include"Channel.h"
#include"Eventloop.h"
#include<iostream>
void Channel::setInterestedInRead(bool on){
    if(on){
        _events|=EPOLLIN;

    }else{
        _events&=~EPOLLIN;

    }
    update();
}
void Channel::setInterestedInWrite(bool on){
    if(on){
        _events|=EPOLLOUT;
    }else{
        _events&=~EPOLLOUT;
    }
    update();
}

void Channel::handleEvent(){
    if(_revents&EPOLLHUP&&!(_revents&EPOLLIN)){
        if(_closecb) _closecb();
    }
    if(_revents&EPOLLERR){
        if(_errorcb) _errorcb();
    }
    if(_revents&EPOLLIN){
        if(_readcb) _readcb();
    }
    if(_revents&EPOLLOUT){
        if(_writecb) _writecb();
    }
}
void Channel::update(){
    _loop->updateChannel(this);
}
