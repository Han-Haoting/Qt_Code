#include "Extract_Region.h"

void Extract_Region()
{
    extern Mat image,binar,cover;
    RNG rng(10086); // 设置随机数种子
    Mat out,stats,centroids; // 用于存放输出图像、不同连通域的统计信息、每个连通域的质心坐标
    int num = connectedComponentsWithStats(binar,out,stats,centroids,8,CV_16U); // 进行连通域划分
    std::vector<Vec3b> colors; // 设置每个连通域的颜色
    Mat result = Mat::zeros(binar.size(),image.type());  // 以不同颜色标记出不同的连通域
    int w = result.cols; // 图像宽
    int h = result.rows; // 图像高
    cover = Mat::zeros(binar.size(),binar.type()); // 用于存放盖板区域
    for (int i=0;i<num;i++)
    {
        if(stats.at<int>(i,4)<100) // 消除面积小于一定阈值的区域
        {
            for (int row=0;row<h;row++)
            {
                for (int col=0;col<w;col++)
                {
                    int label = out.at<int>(row,col);
                    if(label == i)
                    {
                        binar.at<uchar>(row,col) = 255;
                    }
                }
            }
        }
    }
    num = connectedComponentsWithStats(binar,out,stats,centroids,8,CV_16U); // 将孤立像素点消除之后再进行一次连通域分割
    for (int row=0;row<h;row++)
    {
        for (int col=0;col<w;col++)
        {
            int label = out.at<uint16_t>(row,col);
            if(label == 0) // 背景保持不变
            {
                continue;
            }
            if(label == 1) // 提取出VC板部分
            {
                cover.at<uchar>(row,col) = 255;
            }
        }
    }
}
