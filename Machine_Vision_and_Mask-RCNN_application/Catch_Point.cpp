#include "Catch_Point.h"

void Catch_Point()
{
    extern Mat image;
    extern Point2f cover_ctr,cover_p1,cover_p2,cover_p3,cover_p4,P1; // 盖板最小外接矩形的中点、四个角点与侧边轮廓线中点
    extern Point2f cross_pt; // 抓取点
    extern Point2f Mask_Rcnn_Pt1,Mask_Rcnn_Pt2,injection_ctr,Mask_injection_ctr; // 用于存放Mask_RCNN网络的预测框坐标与注液口最小外接矩形的中点
    extern LinePara Cover_LP, Injection_LP;
    if (Cover_LP.k != 0)
    {
        cross_pt.x = (Cover_LP.b - Injection_LP.b) / (Injection_LP.k - Cover_LP.k);
        cross_pt.y = Injection_LP.k * cross_pt.x + Injection_LP.b;
    }
    if (Cover_LP.k == 0)
    {
        cross_pt.x = injection_ctr.x;
        cross_pt.y = cover_ctr.y;
    }
    circle(image, cross_pt, 10, Scalar(0, 255, 0), -1); // 在原图上画出抓取点
    circle(image,injection_ctr,10,Scalar(255,0,0),-1); // 画出最小外接矩形注检测中心点
    circle(image,Mask_injection_ctr,10,Scalar(0,0,255),-1); // 画出Mask_RCNN检测中心点
    circle(image,cover_ctr,10,Scalar(0,0,255),-1); // 画出盖板区域中心点
    line(image, cover_p1, cover_p2, Scalar(0, 0, 255), 3);  // 画出底部轮廓线方程
    line(image, cover_p2, cover_p3, Scalar(0, 0, 255), 3);	// 画出侧边轮廓线方程
    line(image, cover_ctr, P1, Scalar(255, 0, 0), 3);
    line(image, cover_ctr, cross_pt, Scalar(255, 0, 0), 3); // VC板中心点到该点的直线
    line(image, injection_ctr, cross_pt, Scalar(255, 0, 0), 3); // 注液口与交点的直线
}
