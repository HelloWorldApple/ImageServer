#include <iostream>
#include<stdlib.h>
#include<unistd.h>
#include<memory>
#include"../Jnetlib/Eventloop.h"
#include"../Jnetlib/Channel.h"
#include"../Jnetlib/Epoll.h"
#include"../Jnetlib/Socket.h"
#include"../Jnetlib/ThreadPool.h"
using namespace std;
const int SERVER_PORT=1024;
const char* SERVER_IP="127.0.0.1";

int main(int argc, char *argv[])
{
    //int sockfd=CreateNonblockingOrDie(PF_INET);
    Socket socket;
    socket.setReuseAddr(1);
    socket.setReusePort(1);

    Addr serveraddr(SERVER_IP,SERVER_PORT);
    socket.bind(serveraddr);
    socket.listen();

    std::shared_ptr<Epoll> epoller=std::make_shared<Epoll>(10,true);
    Eventloop loop(epoller);
    Channel channel(socket.getsockfd(),&loop);
    channel.setInterestedInRead(true);//will add events into epoll
    int i=1;
    auto readcallback=[&](){
        Addr clientAddr;
        int acceptfd=socket.accept(clientAddr);
        //std::cout<<acceptfd<<std::endl;
        cout<<i++<<endl;
        //Close(acceptfd);

    };
    channel.setReadCallback(readcallback);
    loop.loop();
}

