#include"ImageHash.h"
int PerHash(const cv::Mat& matSrc1,const cv::Mat& matSrc2) {

    //1.缩小尺寸
    //将图片缩小到8x8的尺寸，总共64个像素。
    //这一步的作用是去除图片的细节，只保留结构、明暗等基本信息，摒弃不同尺寸、比例带来的图片差异。
    cv::Mat matDst1, matDst2;

    cv::resize(matSrc1, matDst1, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);
    cv::resize(matSrc2, matDst2, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);

    //2.简化色彩
    //将缩小后的图片，转为64级灰度。也就是说，所有像素点总共只有64种颜色。
    cv::cvtColor(matDst1, matDst1, CV_BGR2GRAY);
    cv::cvtColor(matDst2, matDst2, CV_BGR2GRAY);

    //3.计算平均值
    //计算所有64个像素的灰度平均值。
    int iAvg1 = 0, iAvg2 = 0;
    int arr1[64], arr2[64];

    for (int i = 0; i < 8; i++) {
        uchar* data1 = matDst1.ptr<uchar>(i);
        uchar* data2 = matDst2.ptr<uchar>(i);

        int tmp = i * 8;

        for (int j = 0; j < 8; j++) {
            int tmp1 = tmp + j;

            arr1[tmp1] = data1[j] / 4 * 4;
            arr2[tmp1] = data2[j] / 4 * 4;

            iAvg1 += arr1[tmp1];
            iAvg2 += arr2[tmp1];
        }
    }

    iAvg1 /= 64;
    iAvg2 /= 64;

    //4.比较像素的灰度
    //将每个像素的灰度，与平均值进行比较。大于或等于平均值，记为1；小于平均值，记为0。
    for (int i = 0; i < 64; i++) {
        arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
        arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
    }

    //5.计算哈希值
        //将上一步的比较结果，组合在一起，就构成了一个64位的整数，这就是这张图片的指纹。
    //组合的次序并不重要，只要保证所有图片都采用同样次序就行了。
    int iDiffNum = 0;

    for (int i = 0; i < 64; i++)
        if (arr1[i] != arr2[i])
            ++iDiffNum;

    return iDiffNum;
}
