#include "Binarization.h"

void Binarization()
{
    extern Mat image,binar;
    Mat srcImg; // 存放灰度图像
    cvtColor(image,srcImg,COLOR_BGR2GRAY); // 图像灰度化
    threshold(srcImg,binar,240,255,THRESH_BINARY); // 进行二值化
}
