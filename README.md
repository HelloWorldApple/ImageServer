# OnlineImageProcessing
An online image processing server.
##Feature
Process image


Get image


Save image


##How does it work?
This project has a net library  based on pthread,socket API in Linux.Object oriented threadpool,ConditonVar,Mutex,Epoll,Eventloop are provided.The server is based on event driven model using epoll to meet all clients'request.

request format:method(int)+imagename(namelen(int)+name(char*))+image(row(int)+col(int)+type(int)+MatDatalength(int)+MatData(char*)).

The server parses the request from clients and judges how to process the image by "method" in the request.Then,Server puts the task in the threadpool.When the work finished,server send the image back to client.
##Library
OpenCV 3.1

Pthread

##Platform
Linux
