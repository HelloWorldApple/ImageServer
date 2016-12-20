#include"Functions.h"
#include"Socket.h"
void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


int ReadInt(int acceptfd){
    char buf[sizeof(int)];
    Read(acceptfd,buf,sizeof(int));
    int res;
    memcpy(&res,buf,sizeof(int));

    return res;
}

Mat readMat(int acceptfd){
    int row=ReadInt(acceptfd);
    int col=ReadInt(acceptfd);
    int type=ReadInt(acceptfd);
    int length=ReadInt(acceptfd);

    shared_ptr<uchar> ptr(new uchar[length]);
    ReadAll(acceptfd,ptr.get(),length);
    Mat image=Mat(row,col,type,ptr.get()).clone();
    //delete []data;
    return image;
}
string image2string(const Mat& image){
    uchar* data=image.data;
    size_t count=image.dataend-image.datastart;
    //cout<<count<<endl;
    char rowbuf[sizeof(int)];
    int row=image.rows;
    memcpy(rowbuf,&row,sizeof(row));

    char colbuf[sizeof(int)];
    int col=image.cols;
    memcpy(colbuf,&col,sizeof(col));

    char typebuf[sizeof(int)];
    int type=image.type();
    memcpy(typebuf,&type,sizeof(type));

    char lengthbuf[sizeof(int)];
    memcpy(lengthbuf,&count,sizeof(int));

    string toSend=string(rowbuf,rowbuf+sizeof(int))+
            string(colbuf,colbuf+sizeof(int))+
            string(typebuf,typebuf+sizeof(int))+
            string(lengthbuf,lengthbuf+sizeof(int))+
            string(image.datastart,image.dataend);
    return toSend;
}

string process2string(int method,const string&imagename,const Mat&image){
    if(method==(int)GET){
        char methodbuf[sizeof(int)];
        memcpy(methodbuf,&method,sizeof(method));

        int imagenamelen=imagename.length();
        char namelenbuf[sizeof(int)];
        memcpy(namelenbuf,&imagenamelen,sizeof(int));
        string methodstring=string(methodbuf,sizeof methodbuf);
        string imagenamelenstring=string(namelenbuf,sizeof namelenbuf);
        return methodstring+imagenamelenstring+imagename;
    }else{
        char methodbuf[sizeof(int)];
        memcpy(methodbuf,&method,sizeof(method));

        int imagenamelen=imagename.length();
        char namelenbuf[sizeof(int)];
        memcpy(namelenbuf,&imagenamelen,sizeof(int));

        string methodstring=string(methodbuf,sizeof methodbuf);
        string imagenamelenstring=string(namelenbuf,sizeof namelenbuf);
        string imagestring=image2string(image);
        return methodstring+imagenamelenstring+imagename+imagestring;
    }
}
