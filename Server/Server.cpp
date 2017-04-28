/*#include <iostream>
#include"Functions.h"
#include<stdlib.h>
#include<unistd.h>
#include<memory>
#include<opencv2/opencv.hpp>
#include<unordered_map>
#include"../Jnetlib/Eventloop.h"
#include"../Jnetlib/Channel.h"
#include"../Jnetlib/Epoll.h"
#include"../Jnetlib/Socket.h"
#include"../Jnetlib/ThreadPool.h"
using namespace cv;
using namespace std;
const int SERVER_PORT=2224;
const char* SERVER_IP="127.0.0.1";




//request:method(int)+imagename(namelen(int)+name(char*))+image(row(int)+col(int)+type(int)+MatDatalength(int)+MatData(char*))
unordered_map<int,function<void(const Mat&,const string&,int)>> FunctionMap;


int main(int argc, char *argv[])
{
    //set funciton map
    FunctionMap[(int)MEDIANBLUR]=rgb2gray;
    FunctionMap[(int)SAVE]=saveimage;
    FunctionMap[(int)GET]=getimage;

    //int sockfd=CreateNonblockingOrDie(PF_INET);
    Socket socket;
    int sockfd=socket.getsockfd();
    socket.setReuseAddr(1);
    socket.setReusePort(1);

    Addr serveraddr(SERVER_IP,SERVER_PORT);
    socket.bind(serveraddr);
    socket.listen();

    ThreadPool threadpool(3);

    //Epoll epoller(100,true);
    std::shared_ptr<Epoll> epoller=std::make_shared<Epoll>(100,true);
    Eventloop loop(epoller);
    Channel channel(sockfd,&loop);
    channel.setInterestedInRead(true);//will add events into epoll
    auto readcallback=[&](){
        Addr clientAddr;
        int acceptfd=socket.accept(clientAddr);
        //create a new thread to read data
        threadpool.addTask(
                    Task(
                        [=](){
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
    };
    channel.setReadCallback(readcallback);
    loop.loop();
}*/

/*
#include <iostream>
#include"Functions.h"
#include<stdlib.h>
#include<unistd.h>
#include<memory>
#include<opencv2/opencv.hpp>
#include<unordered_map>
#include"../Jnetlib/Eventloop.h"
#include"../Jnetlib/Channel.h"
#include"../Jnetlib/Epoll.h"
#include"../Jnetlib/Socket.h"
#include"../Jnetlib/ThreadPool.h"
using namespace cv;
using namespace std;
const int SERVER_PORT=2224;
const char* SERVER_IP="127.0.0.1";




//request:method(int)+imagename(namelen(int)+name(char*))+image(row(int)+col(int)+type(int)+MatDatalength(int)+MatData(char*))
unordered_map<int,function<void(const Mat&,const string&,int)>> FunctionMap;


int main(int argc, char *argv[])
{
    //set funciton map
    FunctionMap[(int)MEDIANBLUR]=medianblur;
    FunctionMap[(int)SAVE]=saveimage;
    FunctionMap[(int)GET]=getimage;

    //int sockfd=CreateNonblockingOrDie(PF_INET);
    Socket socket;
    int sockfd=socket.getsockfd();
    socket.setReuseAddr(1);
    socket.setReusePort(1);

    Addr serveraddr(SERVER_IP,SERVER_PORT);
    socket.bind(serveraddr);
    socket.listen();

    ThreadPool threadpool(3);

    //Epoll epoller(100,true);
    std::shared_ptr<Epoll> epoller=std::make_shared<Epoll>(100,1);//use LT!!!!!!!!!!!!!
    Eventloop loop(epoller);
    Channel channel(sockfd,&loop);
    channel.setInterestedInRead(true);//will add events into epoll

    int acceptfd;
    Channel readchannel;

    auto listencallback=[&](){
        cout<<"new connect"<<endl;
        Addr clientAddr;
        acceptfd=socket.accept(clientAddr);//should not local var,catch by lambda keep living


//
        //!!!!!!!!!!!!!!!!!
        //SetNonBlock(acceptfd);//?????why cannot nonblock fd????

        readchannel.set_fd_loop(acceptfd,&loop);//must global,chatch by lamda

        readchannel.setInterestedInRead(true);

        auto readcallback=[&](){
            cout<<"new task"<<endl;

            readchannel.setInterestedInRead(0);
            loop.removeChannel(&readchannel);//advoid wakeup epoll more than 1 times
            threadpool.addTask(
                        Task(
                            [&](){
                            int method=ReadInt(acceptfd);

                            string imagename=ReadString(acceptfd);


                            Mat image;
                            if(method!=(int)GET){
                                image=readMat(acceptfd);
                            }

                            FunctionMap[method](image,imagename,acceptfd);

                            }
                            )
                        );
        };


        readchannel.setReadCallback(readcallback);

    };
    channel.setReadCallback(listencallback);
    loop.loop();
}*/
/*
#include <iostream>
#include"Functions.h"
#include<stdlib.h>
#include<unistd.h>
#include<memory>
#include<opencv2/opencv.hpp>
#include<unordered_map>
#include"../Jnetlib/Eventloop.h"
#include"../Jnetlib/Channel.h"
#include"../Jnetlib/Epoll.h"
#include"../Jnetlib/Socket.h"
#include"../Jnetlib/ThreadPool.h"
using namespace cv;
using namespace std;
const int SERVER_PORT=2224;
const char* SERVER_IP="127.0.0.1";




//request:method(int)+imagename(namelen(int)+name(char*))+image(row(int)+col(int)+type(int)+MatDatalength(int)+MatData(char*))
unordered_map<int,function<void(const Mat&,const string&,int)>> FunctionMap;


int main(int argc, char *argv[])
{
    //set funciton map
    FunctionMap[(int)MEDIANBLUR]=medianblur;
    FunctionMap[(int)SAVE]=saveimage;
    FunctionMap[(int)GET]=getimage;

    //int sockfd=CreateNonblockingOrDie(PF_INET);
    Socket socket;
    int sockfd=socket.getsockfd();
    socket.setReuseAddr(1);
    socket.setReusePort(1);

    Addr serveraddr(SERVER_IP,SERVER_PORT);
    socket.bind(serveraddr);
    socket.listen();

    ThreadPool threadpool(3);

    //Epoll epoller(100,true);
    std::shared_ptr<Epoll> epoller=std::make_shared<Epoll>(100,0);//use LT!!!!!!!!!!!!!
    Eventloop loop(epoller);
    Channel channel(sockfd,&loop);
    channel.setInterestedInRead(true);//will add events into epoll

    int acceptfd;
    Channel readchannel;

    auto listencallback=[&](){
        cout<<"new connect"<<endl;
        Addr clientAddr;
        acceptfd=socket.accept(clientAddr);//should not local var,catch by lambda keep living


        //!!!!!!!!!!!!!!!!!
        //SetNonBlock(acceptfd);//?????why cannot nonblock fd????

        readchannel.set_fd_loop(acceptfd,&loop);//must global,chatch by lamda

        readchannel.setInterestedInRead(true);

        auto readcallback=[&](){
            cout<<"new task"<<endl;

            readchannel.setInterestedInRead(0);
            loop.removeChannel(&readchannel);//advoid wakeup epoll more than 1 times

            threadpool.addTask(
                        Task(
                            [&](){
                                int method=ReadInt(acceptfd);

                                string imagename=ReadString(acceptfd);
                                //string taskname=to_string(method)+imagename;


                                Mat image;
                                if(method!=(int)GET){
                                    image=readMat(acceptfd);
                                }
                                //it will search weather there is a same task result.

                                FunctionMap[method](image,imagename,acceptfd);
                            }

                            )
                        );
        };


        readchannel.setReadCallback(readcallback);

    };
    channel.setReadCallback(listencallback);
    loop.loop();
}*/


#include <iostream>
#include"Functions.h"
#include<stdlib.h>
#include<unistd.h>
#include<memory>
#include<opencv2/opencv.hpp>
#include<unordered_map>
#include"../Jnetlib/Eventloop.h"
#include"../Jnetlib/Channel.h"
#include"../Jnetlib/Epoll.h"
#include"../Jnetlib/Socket.h"
#include"../Jnetlib/ThreadPool.h"
#include"../Jnetlib/LRUCache.h"
using namespace cv;
using namespace std;
const int SERVER_PORT=2224;
const char* SERVER_IP="127.0.0.1";




//request:method(int)+imagename(namelen(int)+name(char*))+image(row(int)+col(int)+type(int)+MatDatalength(int)+MatData(char*))
unordered_map<int,function<void(const Mat&,const string&,int,LRUCache<string,Mat>&)>> FunctionMap;


int main(int argc, char *argv[])
{
    //set funciton map
    FunctionMap[(int)MEDIANBLUR]=medianblur;
    FunctionMap[(int)SAVE]=saveimage;
    FunctionMap[(int)GET]=getimage;

    //int sockfd=CreateNonblockingOrDie(PF_INET);
    Socket socket;
    int sockfd=socket.getsockfd();
    socket.setReuseAddr(1);
    socket.setReusePort(1);

    Addr serveraddr(SERVER_IP,SERVER_PORT);
    socket.bind(serveraddr);
    socket.listen();

    ThreadPool threadpool(4);
    LRUCache<string,Mat> imagepool(100);
    std::shared_ptr<Epoll> epoller=std::make_shared<Epoll>(100,1);
    Eventloop loop(epoller);
    Channel channel(sockfd,&loop);
    channel.setInterestedInRead(true);//will add events into epoll

    int acceptfd;
    Channel readchannel;//must global,chatch by lamda

    auto listencallback=[&](){
        cout<<"new connect"<<endl;
        Addr clientAddr;
        acceptfd=socket.accept(clientAddr);//should not local var,catch by lambda keep living

        //!!!!!!!!!!!!!!!!!
        SetNonBlock(acceptfd);//

        readchannel.set_fd_loop(acceptfd,&loop);

        readchannel.setInterestedInRead(true);

        auto readcallback=[&](){
            cout<<"new task"<<endl;

            readchannel.setInterestedInRead(0);
            loop.removeChannel(&readchannel);//advoid wakeup epoll more than 1 times

            threadpool.addTask(
                        Task(
                            [&](){
                                int method=ReadInt(acceptfd);

                                string imagename=ReadString(acceptfd);
                                //string taskname=to_string(method)+imagename;


                                Mat image;
                                if(method!=(int)GET){
                                    image=readMat(acceptfd);
                                    //imagepool.put(imagename,image);
                                }
                                //it will search weather there is a same task result.

                                FunctionMap[method](image,imagename,acceptfd,imagepool);
                            }

                            )
                        );
        };


        readchannel.setReadCallback(readcallback);

    };
    channel.setReadCallback(listencallback);
    loop.loop();
}



/*
#include<LRUCache.h>
#include<iostream>
using namespace std;
int main(){
    LRUCache<int,char> l(2);
    l.put(1,'a');
    l.put(2,'b');
    cout<<l.get(1)<<endl;//a
    l.put(3,'c');
    cout<<l.get(2)<<endl;//0
    l.put(4,'d');
    cout<<l.get(1)<<endl;//0
    cout<<l.get(3)<<endl;//c
    cout<<l.get(4)<<endl;//d
}*/

/*
#include"ImageHash.h"
#include <string>
using namespace cv;
using namespace std;
int main() {

    string ImageName1 = "lena.jpg";
    string ImageName2="medianblur_512_512lena.jpg";
    Mat Image1,Image2;
    Image1=cv::imread(ImageName1);
    Image2=cv::imread(ImageName2);
    int iDiffNum = PerHash(Image1,Image2);

    cout << "iDiffNum = " << iDiffNum << endl;


    //得到指纹以后，就可以对比不同的图片，看看64位中有多少位是不一样的。
    //在理论上，这等同于计算"汉明距离"（Hamming distance）。
    //如果不相同的数据位不超过5，就说明两张图片很相似；如果大于10，就说明这是两张不同的图片。
    if (iDiffNum <= 5)
        cout << "two images are very similar!" << endl;
    else if (iDiffNum > 10)
        cout << "they are two different images!" << endl;
    else
        cout << "two image are somewhat similar!" << endl;
    //char ch = getchar();
}*/
