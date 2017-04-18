# OnlineImageProcessing

An online image processing server.

Feature
--
Event Driven

LRUCache Imagepool

Threadpool


How does it work?
--
This project has a net library  based on pthread,socket API in Linux. Threadpool,ConditonVar,Mutex,Epoll,Eventloop are provided.The server is based on event driven model using epoll to meet all clients'request.The server parses the request from clients and judges how to process the image by "method" in the request.Then,Server puts the task in the threadpool.When the work finished,server send the image back to client.To reduce I/O cost,LRU imagepool is used.

request format
--
method(int)+imagename(namelen(int)+name(char*))+image(row(int)+col(int)+type(int)+MatDatalength(int)+MatData(char*)).

Get start
--
There is a medianblur example in dir 'Client' and 'Server'.Follow steps:

cd Server

cmake .

make

./Server&

cd ../Client

cmake .

make

./Client

Library
--
OpenCV 3.1

Pthread

Platform
--
Linux
