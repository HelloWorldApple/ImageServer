#include <iostream>
#include<stdlib.h>
#include<unistd.h>
#include<memory>
#include"Eventloop.h"
#include"Channel.h"
#include"Epoll.h"
#include"Socket.h"
#include"ThreadPool.h"
#include<ctime>
using namespace std;

const int SERVER_PORT=1024;
const char* SERVER_IP="127.0.0.1";



int main(int argc, char *argv[])
{
    const int N=50000;
    time_t start=clock();
    for(int i=0;i<N;i++){
        {
            int sockfd=CreateNonblockingOrDie(PF_INET);
            Socket socket(sockfd);
            socket.setReuseAddr(1);
            socket.setReusePort(1);
            Addr serverAddr(SERVER_IP,SERVER_PORT);
            socket.connect(serverAddr);
        }
    }
    double usetime=(clock()-start)/CLOCKS_PER_SEC;
    std::cout<<usetime<<endl;
}
