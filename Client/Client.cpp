#include <iostream>
#include<sstream>
#include<memory>
#include"Epoll.h"
#include"Channel.h"
#include"Eventloop.h"
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



//request:method(int)+imagename(namelen(int)+name(char*))+image(row(int)+col(int)+type(int)+MatDatalength(int)+MatData(char*))
int main(int argc, char *argv[])
{

    argc=3;
    if(argc!=3){
        cerr<<"useage:./Client METHOD(0:medianblur\1:save) IMAGENAME";
        return 1;
    }
    string imagename="lena.jpg";//;argv[2];
    METHOD methodname=GET;//argv[1];

   // int sockfd=CreateNonblockingOrDie(PF_INET);
    Socket socket;
    int sockfd=socket.getsockfd();
    socket.setReuseAddr(1);
    socket.setReusePort(1);
    Addr serverAddr(SERVER_IP,SERVER_PORT);
    socket.connect(serverAddr);

    Mat image;
    string requeststring;
    switch(methodname){
    case MEDIANBLUR:{

        image=imread(imagename);
        requeststring=process2string((int)methodname,imagename,image);
        WriteAll(sockfd,requeststring.data(),requeststring.size());

        std::shared_ptr<Epoll> epoller=std::make_shared<Epoll>(1);
        Eventloop loop(epoller);
        Channel c(sockfd,&loop);
        c.setReadCallback([&]()->int{
            Mat image=readMat(sockfd);
            string windowname="res";
            namedWindow(windowname);
            imshow(windowname,image);
            waitKey(1000);
            loop.quit();
            return 0;
        });
        c.setInterestedInRead(true);
        loop.loop();
        break;
        /*//old version:no eventloop and no channel
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
                    waitKey(1000);
                    return 0;
                }
            }
        }
        break;*/
    }//case
    case SAVE:{
        image=imread(imagename);
        requeststring=process2string((int)methodname,imagename,image);

        WriteAll(sockfd,requeststring.data(),requeststring.size());

        break;
    }//case
    case GET:{
        requeststring=process2string((int)methodname,imagename,Mat());
        WriteAll(sockfd,requeststring.data(),requeststring.size());
        std::shared_ptr<Epoll> epoller=std::make_shared<Epoll>(1);
        Eventloop loop(epoller);
        Channel c(sockfd,&loop);
        c.setReadCallback([&]()->int{
              Mat image=readMat(sockfd);
              string windowname=imagename;
              namedWindow(windowname);
              imshow(windowname,image);
              waitKey(1000);
              loop.quit();
              return 0;
        });
        c.setInterestedInRead(true);
        loop.loop();
        break;
        /*
        Epoll epoller(5);
        epoller.add(sockfd,EPOLLIN);

        while(1){
            cout<<"inter loop"<<endl;
            int eventcount=epoller.wait(10);
            const struct epoll_event *events=epoller.getEvents();
            for(int i=0;i<eventcount;i++){
                if(events[i].data.fd==sockfd){
                    Mat image=readMat(sockfd);
                    string windowname=imagename;
                    namedWindow(windowname);
                    imshow(windowname,image);
                    waitKey(1000);
                    return 0;
                }
            }
        }
        break;*/
    }//case
    }//switch


}
