#include <iostream>
#include<stdlib.h>
#include<unistd.h>
#include<memory>
#include"../Jnetlib/Eventloop.h"
#include"../Jnetlib/Channel.h"
#include"../Jnetlib/Epoll.h"
#include"../Jnetlib/Socket.h"
#include"../Jnetlib/ThreadPool.h"
#include<ctime>
using namespace std;

const int SERVER_PORT=1024;
const char* SERVER_IP="127.0.0.1";



int main(int argc, char *argv[])
{
    const int N=1;
    auto func=[](){
        Socket socket;
        socket.setReuseAddr(1);
        socket.setReusePort(1);
        Addr serverAddr(SERVER_IP,SERVER_PORT);
        socket.connect(serverAddr);
    };
    vector<Socket> v(N);
    Addr serverAddr(SERVER_IP,SERVER_PORT);
    //time_t start=clock();
    for(int i=0;i<N;i++){
        v[i].setReuseAddr(1);
        v[i].setReusePort(1);
        //func();
        v[i].connect(serverAddr);

    }
    //double usetime=(clock()-start)/CLOCKS_PER_SEC;
    //std::cout<<usetime<<endl;
}
