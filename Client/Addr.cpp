#include"Addr.h"
Addr::Addr(const char* ip,int port):_ip(ip),_port(port){
    _addr.sin_family=AF_INET;
    _addr.sin_port=htons(port);
    _addr.sin_addr.s_addr=inet_addr(ip);
}

Addr::~Addr(){

}
