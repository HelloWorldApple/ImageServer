# OnlineImageProcessing
An online image processing server.
##Feature
Process image

Get image

Save image

##How it work?
This project has a net library  based on pthread,socket on Linux.Object oriented threadpool,ConditonVar,Mutex,Epoll,Socket are provided.The server is based on event reactor model using epoll to meet all clients'request.

request format:method(int)+imagename(namelen(int)+name(char*))+image(row(int)+col(int)+type(int)+MatDatalength(int)+MatData(char*)).

The server parse the request from client and judge how to process the image by "method".Then,it put the work into the threadpool.After image processed,the server will send the image back to the client.
##Library
OpenCV 3.1
