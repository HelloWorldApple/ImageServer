#ifndef SOCKET_H
#define SOCKET_H
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<netinet/tcp.h>
#include<assert.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include"Addr.h"
using SA=struct sockaddr;

void SetNonBlock(int sockfd);
void SetCloseOnExec(int sockfd);

int CreateNonblockingOrDie(sa_family_t family);

void BindOrDie(int sockfd,const SA* addr);
void BindOrDie(int sockfd,const Addr& addr);
void ListenOrDie(int sockfd);

int Accept(int sockfd,struct sockaddr_in6* addr);
int Accept(int sockfd,Addr& addr);

int Connect(int sockfd,const SA* addr);
int Connect(int sockfd,const Addr& addr);

ssize_t Read(int sockfd,void *buf,size_t count);
ssize_t ReadAll(int sockfd,void *buf,size_t count);
//ssize_t Readv(int sockfd,const struct iovec *iov,int iovcnt);

ssize_t Write(int sockfd,const void* buf,size_t count);
ssize_t WriteAll(int sockfd,const void* buf,size_t count);

void Close(int sockfd);
void ShutdownWrite(int sockfd);

int GetSocketError(int sockfd);

struct sockaddr_in6 GetLocalAddr(int sockfd);

struct sockaddr_in6 GetPeerAddr(int sockfd);


class Socket{
public:
     Socket();
    ~Socket();

    void bind(const struct sockaddr_in* addr);
    void bind(const Addr& addr);
    void listen();
    int connect(const SA* addr);
    int connect(const Addr& addr);
    int accept( struct sockaddr_in* addr);
    int accept(Addr& addr);
    void shutdownWrite();
    void setNoDelay(bool);
    void setReuseAddr(bool);
    void setReusePort(bool);
    void setKeepAlive(bool);

    int getsockfd(){return _sockfd;}
private:
    int _sockfd;
};
#endif
