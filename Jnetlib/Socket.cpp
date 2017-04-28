#include"Socket.h"
#include<error.h>
#include<errno.h>
void SetNonBlock(int sockfd){
    int flags=fcntl(sockfd,F_GETFL,0);
    flags|=O_NONBLOCK;
    fcntl(sockfd,F_SETFL,flags);
}
void SetCloseOnExec(int sockfd){
    int flags=fcntl(sockfd,F_GETFD,0);
    flags|=FD_CLOEXEC;
    fcntl(sockfd,F_SETFL,flags);
}

int CreateNonblockingOrDie(sa_family_t family){
    int sockfd=socket(family,SOCK_STREAM,IPPROTO_TCP);
    assert(sockfd>=0);
    SetNonBlock(sockfd);
    SetCloseOnExec(sockfd);
    return sockfd;
}

void BindOrDie(int sockfd,const SA* addr){
    int ret = bind(sockfd,addr,socklen_t(sizeof(SA)));
    assert(ret>=0);

}
void BindOrDie(int sockfd,const Addr& addr){
    const SA* paddr=(const SA*)const_cast<sockaddr_in*>(&addr._addr);
    int ret = bind(sockfd,paddr,socklen_t(sizeof(SA)));
    assert(ret>=0);

}
void ListenOrDie(int sockfd){
    int ret=listen(sockfd,SOMAXCONN);//max listen queue size 128
    assert(ret>=0);
}

int Accept(int sockfd,struct sockaddr_in* addr){
    socklen_t addrlen=static_cast<socklen_t>(sizeof(*addr));
    int connfd=accept(sockfd,(SA*)(addr),&addrlen);
    SetNonBlock(connfd);
    SetCloseOnExec(connfd);
    assert(connfd);
    return connfd;
}

int Accept(int sockfd,Addr& addr){

    socklen_t addrlen=static_cast<socklen_t>(sizeof(SA));
    int connfd=accept(sockfd,(SA*)(&addr._addr),&addrlen);
    SetNonBlock(connfd);
    SetCloseOnExec(connfd);
    assert(connfd);
    return connfd;
}

int Connect(int sockfd,const SA* addr){
    return connect(sockfd,addr,(socklen_t)sizeof(struct sockaddr_in));
}

int Connect(int sockfd,const Addr& addr){

    return connect(sockfd,(const SA*)const_cast<sockaddr_in*>(&addr._addr),(socklen_t)sizeof(struct sockaddr_in));
}

ssize_t Read(int sockfd,void *buf,size_t count){
    return read(sockfd,buf,count);
}
ssize_t ReadAll(int sockfd,void *buf,size_t count){
    ssize_t readed=0;
    while(readed!=count){
        ssize_t onceRead=read(sockfd,buf,count-readed);
        if(onceRead==-1){
            if(errno==EINTR){
                //onceRead=0;
                continue;
            }else if(errno==EAGAIN){
                //onceRead=0;
                continue;
            }
            else return -1;
        }
        else if(onceRead==0) break;//EOF

        readed+=onceRead;
        buf+=onceRead;
    }
    return readed;
}

//ssize_t Readv(int sockfd,const struct iovec *iov,int iovcnt){
 //   return readv(sockfd,iov,iovcnt);
//}

ssize_t Write(int sockfd,const void* buf,size_t count){
    return write(sockfd,buf,count);
}

ssize_t WriteAll(int sockfd,const void* buf,size_t count){
    ssize_t written=0;
    while(written!=count){
        ssize_t onceWritten=write(sockfd,buf,count-written);
        if(onceWritten==-1){
            if(errno==EINTR){
                //onceWritten=0;
                continue;
            }else if(errno==EAGAIN){
                //onceWritten=0;
                continue;
            }
            else{
                return -1;
            }
        }
        else if(onceWritten==0)  break;//
        written+=onceWritten;
        buf+=onceWritten;
    }
    return written;
}


void Close(int sockfd){
    int ret=close(sockfd);
    assert(ret>=0);
}
void ShutdownWrite(int sockfd){
    int ret=shutdown(sockfd,SHUT_WR);
    assert(ret>=0);
}

int GetSocketError(int sockfd){
    int optval;
    socklen_t optlen=sizeof(optval);
    if(getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&optval,&optlen)<0){
        return errno;
    }else{
        return optval;
    }
}

struct sockaddr_in6 GetLocalAddr(int sockfd){
    struct sockaddr_in6 localaddr;
    bzero(&localaddr,sizeof localaddr);
    socklen_t addrlen=(socklen_t)sizeof localaddr;
    int ret=getsockname(sockfd,( SA*)&localaddr,&addrlen);
    assert(ret>=0);
    return localaddr;
}

struct sockaddr_in6 GetPeerAddr(int sockfd){
    struct sockaddr_in6 peeraddr;
    bzero(&peeraddr,sizeof peeraddr);
    socklen_t addrlen=(socklen_t)sizeof peeraddr;
    int ret=getsockname(sockfd,( SA*)&peeraddr,&addrlen);
    assert(ret>=0);
    return peeraddr;
}


Socket::Socket( ):_sockfd(CreateNonblockingOrDie(PF_INET)){

}

Socket::~Socket(){
    Close(_sockfd);
}

void Socket::bind(const struct sockaddr_in* addr){
    BindOrDie(_sockfd,(const SA*)addr);
}
void Socket::bind(const Addr& addr){
    BindOrDie(_sockfd,addr);
}


void Socket::listen(){
    ListenOrDie(_sockfd);
}
int Socket::connect(const SA* addr){
    return Connect(_sockfd,addr);
}
int Socket::connect(const Addr& addr){
    return Connect(_sockfd,addr);
}

int Socket::accept(struct sockaddr_in* addr){
    bzero(addr,sizeof(*addr));
    int connfd=Accept(_sockfd,addr);
    return connfd;
}

int Socket::accept(Addr& addr){
    bzero(&addr._addr,sizeof(sockaddr_in));
    int connfd=Accept(_sockfd,addr);
    return connfd;
}

void Socket::shutdownWrite(){
    ShutdownWrite(_sockfd);
}
void Socket::setNoDelay(bool on){
    int optval=on?1:0;
    setsockopt(_sockfd,IPPROTO_TCP,TCP_NODELAY,&optval,(socklen_t)sizeof optval);
}

void Socket::setReuseAddr(bool on){
    int optval=on?1:0;
    setsockopt(_sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,(socklen_t)sizeof optval);
}


void Socket::setReusePort(bool on){
    int optval=on?1:0;
    setsockopt(_sockfd,SOL_SOCKET,SO_REUSEPORT,&optval,(socklen_t)sizeof optval);

}
void Socket::setKeepAlive(bool on){
    int optval=on?1:0;
    setsockopt(_sockfd,SOL_SOCKET,SO_KEEPALIVE,&optval,(socklen_t)sizeof optval);
}
