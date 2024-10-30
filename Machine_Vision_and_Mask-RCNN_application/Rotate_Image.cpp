#include "Rotate_Image.h"

void Rotate_Image()
{
    extern Mat image,rotate_image;
    extern double angle;
    //旋转截断问题
    double alpha = -angle * CV_PI / 180.0; //将角度转换成弧度制
    Point2f srcP[3];
    Point2f dstP[3];
    srcP[0] = cv::Point2f(0, image.rows);
    srcP[1] = cv::Point2f(image.cols, 0);
    srcP[2] = cv::Point2f(image.cols, image.rows);
    //rotate the pixels
    for (int i = 0; i < 3; i++)
    {
        dstP[i] = cv::Point2f(srcP[i].x*cos(alpha) - srcP[i].y*sin(alpha), srcP[i].y*cos(alpha) + srcP[i].x*sin(alpha));
    }
    double minx, miny, maxx, maxy;
    minx = std::min(std::min(std::min(dstP[0].x, dstP[1].x), dstP[2].x), float(0.0));
    miny = std::min(std::min(std::min(dstP[0].y, dstP[1].y), dstP[2].y), float(0.0));
    maxx = std::max(std::max(std::max(dstP[0].x, dstP[1].x), dstP[2].x), float(0.0));
    maxy = std::max(std::max(std::max(dstP[0].y, dstP[1].y), dstP[2].y), float(0.0));
    int w = maxx - minx;
    int h = maxy - miny;
    //translation
//    for (int i = 0; i < 3; i++)
//    {
//        if (minx < 0)
//        dstP[i].x -= minx;
//        if (miny < 0)
//        dstP[i].y -= miny;
//    }
//    Mat warpMat = cv::getAffineTransform(srcP, dstP);
//    warpAffine(image, rotate_image, warpMat, cv::Size(w, h));
    cv::Mat warpMat =cv::Mat::zeros(cv::Size(3,2),CV_64F);//rows=2,cols=3

    std::cout << warpMat.type() << std::endl;
    std::cout << warpMat.size()<<std::endl;
    warpMat.at<double>(0, 0) = cos(alpha);
    warpMat.at<double>(0, 1) = 0- sin(alpha);
    warpMat.at<double>(1, 0) = sin(alpha);
    warpMat.at<double>(1, 1) = cos(alpha);
    warpMat.at<double>(0, 2) =0- minx;
    warpMat.at<double>(1, 2) =0-miny;
    //std::cout << warpMat;
    cv::warpAffine(image, rotate_image, warpMat, cv::Size(w, h));//extend size

}
