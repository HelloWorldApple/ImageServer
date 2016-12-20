#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include<vector>
#include<sstream>
#include<opencv2/opencv.hpp>
#include<memory>
#include<string>
using std::string;
using std::shared_ptr;
using cv::Mat;

enum METHOD{MEDIANBLUR,SAVE,GET};
void split(const std::string &s,char delim,std::vector<std::string>&elems);
std::vector<std::string> split(const std::string &s,char delim);
int ReadInt(int acceptfd);
Mat readMat(int acceptfd);
string image2string(const Mat& image);
string process2string(int method,const string&imagename,const Mat&image);


#endif
