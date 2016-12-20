#include <iostream>
#include"Epoll.h"
#include"Socket.h"
#include"ThreadPool.h"
#include"Functions.h"
#include<stdlib.h>
#include<unistd.h>
#include<memory>
#include<opencv2/opencv.hpp>
#include<unordered_map>
using namespace cv;
using namespace std;
const int SERVER_PORT=2224;
const char* SERVER_IP="127.0.0.1";




//request:method(int)+imagename(namelen(int)+name(char*))+image(row(int)+col(int)+type(int)+MatDatalength(int)+MatData(char*))
unordered_map<int,function<void(const Mat&,const string&,int)>> FunctionMap;


int main(int argc, char *argv[])
{
    /*set funciton map*/
    FunctionMap[(int)MEDIANBLUR]=rgb2gray;
    FunctionMap[(int)SAVE]=saveimage;
    FunctionMap[(int)GET]=getimage;

    int sockfd=CreateNonblockingOrDie(PF_INET);
    Socket socket(sockfd);
    socket.setReuseAddr(1);
    socket.setReusePort(1);

    Addr serveraddr(SERVER_IP,SERVER_PORT);
    socket.bind(serveraddr);
    socket.listen();

    Epoll epoller(100,true);
    epoller.add(sockfd,EPOLLIN);
    ThreadPool threadpool(5);

    while(1){
        cout<<"inter loop"<<endl;
        int eventcount=epoller.wait(-1);
        const struct epoll_event *events=epoller.getEvents();
        for(int i=0;i<eventcount;i++){
            if(events[i].data.fd==sockfd){
                cout<<"receive request"<<endl;
                Addr clientAddr;
                int acceptfd=socket.accept(clientAddr);

                //create a new thread to read data

                threadpool.addTask(
                            Task(
                                [&](){
                                int method=ReadInt(acceptfd);
                                string imagename=ReadString(acceptfd);
                                Mat image;
                                if(method!=(int)GET)
                                    image=readMat(acceptfd);
                                FunctionMap[method](image,imagename,acceptfd);
                                //threadpool.addTask(Task(std::bind(FunctionMap[method],image,acceptfd,fd[1])));
                                }
                                )
                            );
            }
        }
    }
}


