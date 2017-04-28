#include"Functions.h"
#include"../Jnetlib/Socket.h"
#include"../Jnetlib/LRUCache.h"
#include"ImageHash.h"
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
string ReadString(int acceptfd){
    const int len=ReadInt(acceptfd);
    char buf[len];
    ReadAll(acceptfd,buf,len);
    return string(buf,len);
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
    string res;
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

bool existFile(const string& filename,Mat& res){
    try{
        res=cv::imread(filename);
    }catch(...){
        return false;
    }

    return res.data!=nullptr;
}

void medianblur(const Mat& image,const string& imagename,int acceptfd,LRUCache<string,Mat>& imagepool){
    std::cout<<"medianblur function"<<std::endl;
    //Mat sendImg;
    /*if(existFile("medianblur_"+std::to_string(image.rows)+"_"+std::to_string(image.cols)+imagename,sendImg)){
        string toSend=image2string(sendImg);
        WriteAll(acceptfd,toSend.data(),toSend.size());
    }else{
        cv::medianBlur(image,image,11);
        cv::imwrite("medianblur_"+std::to_string(image.rows)+"_"+std::to_string(image.cols)+imagename,image);
        string toSend=image2string(image);
        WriteAll(acceptfd,toSend.data(),toSend.size());
    }*/
    string filename="medianblur_"+std::to_string(image.rows)+"_"+std::to_string(image.cols)+imagename;
    string toSend;
    if(imagepool.contain(filename)&&PerHash(image,imagepool.get(filename))<10){//file hash to identify image which has same imagename
        cout<<"pool contain!"<<endl;
        Mat sendImg=imagepool.get(filename);
        string toSend=image2string(sendImg);
        WriteAll(acceptfd,toSend.data(),toSend.size());
    }else{
        cout<<"pool not contain!"<<endl;
        Mat res;
        if(existFile(filename,res)){
            cv::waitKey(1000);//cost time
            std::cout<<"exist file"<<std::endl;
            imagepool.put(filename,res);
            toSend=image2string(res);
        }else{
            std::cout<<"do task"<<std::endl;
            cv::waitKey(1000);//cost time
            cv::medianBlur(image,res,11);
            cv::imwrite(filename,res);
            imagepool.put(filename,res);
            toSend=image2string(res);
        }

        WriteAll(acceptfd,toSend.data(),toSend.size());
    }



}

void saveimage(const Mat& image,const string& imagename,int acceptfd,LRUCache<string,Mat>& imagepool){
    cv::imwrite(imagename,image);
    string toSend="save success";
    WriteAll(acceptfd,toSend.data(),toSend.size());
}
void getimage(const Mat& image,const string& imagename,int acceptfd,LRUCache<string,Mat>& imagepool){
    (void)image;
    Mat img=cv::imread(imagename);
    string toSend=image2string(img);
    WriteAll(acceptfd,toSend.data(),toSend.size());

}


