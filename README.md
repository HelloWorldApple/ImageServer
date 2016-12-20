# OnlineImageProcessing
An online image processing server.The clients send a image to the server which processed the image and send it back.
##How it work?
This project has a net library  based on pthread,socket on Linux.In the net library,An object oriented threadpool,ConditonVar,Mutex,Epoll,Socket and so on.  The server works in event reactor model that using epoll to meet all clients'request.

request:method(int)+imagename(namelen(int)+name(char*))+image(row(int)+col(int)+type(int)+MatDatalength(int)+MatData(char*)).

The server parse the request from client and judge how to process the image by "method".Then,it put the work into the threadpool.After image processed,the server will send the image back to the client.
##Library
OpenCV 3.1
