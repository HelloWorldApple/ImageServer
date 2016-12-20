#include <iostream>
#include<sstream>
#include<memory>
#include"Epoll.h"
#include"Socket.h"
#include<sys/sendfile.h>
#include<sys/stat.h>
#include <opencv2/opencv.hpp>
#include"Functions.h"
using namespace std;
using namespace cv;
const int SERVER_PORT=2224;
const char* SERVER_IP="127.0.0.1";
const int MAXFILESIZE=1000000;




int main(int argc, char *argv[])
{

    int sockfd=CreateNonblockingOrDie(PF_INET);
    Socket socket(sockfd);
    socket.setReuseAddr(1);
    socket.setReusePort(1);
    Addr serverAddr(SERVER_IP,SERVER_PORT);

    socket.connect(serverAddr);

    Mat image=imread("lena.jpg");
//request:method(int)+image(row(int)+col(int)+type(int)+MatDatalength(int)+MatData(char*))
    string requeststring=process2string((int)MEDIANBLUR,image);

    //cout<<imagestring.size();
    for(int i=0;i<4;i++){
        WriteAll(sockfd,requeststring.data(),requeststring.size());
    }

    Epoll epoller(5);
    epoller.add(sockfd,EPOLLIN);

    while(1){
        cout<<"inter loop"<<endl;
        int eventcount=epoller.wait(10);
        const struct epoll_event *events=epoller.getEvents();
        for(int i=0;i<eventcount;i++){
            if(events[i].data.fd==sockfd){
                Mat image=readMat(sockfd);
                string windowname="res"+to_string(i);
                namedWindow(windowname);
                imshow(windowname,image);
                waitKey(100);
            }
        }
    }


}
