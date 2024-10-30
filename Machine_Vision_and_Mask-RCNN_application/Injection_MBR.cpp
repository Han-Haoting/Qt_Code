#include "Injection_MBR.h"

void Injection_MBR()
{
    extern Mat detection_result, detection_result_edge, detection_result_RGB;
    extern LinePara Cover_LP,Injection_LP;
    extern Point2f Mask_Rcnn_Pt1,Mask_Rcnn_Pt2,injection_ctr,Mask_injection_ctr; // 用于存放Mask_RCNN网络的预测框坐标与注液口最小外接矩形的中点
    vector<vector<Point>> st_contours;
    vector<Vec4i> st_hierarchy;
    findContours(detection_result_edge, st_contours, st_hierarchy, 0, 2, Point());// 轮廓发现与绘制
    RotatedRect st_rrect;
    st_rrect = minAreaRect(st_contours[1]); // 寻找轮廓的外接矩形
    Point2f st_points[4];
    st_rrect.points(st_points); // 读取最小外接矩形的4个顶点
    if (st_rrect.size.width / st_rrect.size.height <= 1) // 做矩形的长宽判断，以此来确定外接矩形下轮廓的两点，进而求出底部轮廓线的中点
    {
        injection_ctr.x = (st_points[0].x + st_points[3].x) / 2;
        injection_ctr.y = (st_points[0].y + st_points[3].y) / 2;
    }
    if (st_rrect.size.width / st_rrect.size.height > 1)
    {
        injection_ctr.x = (st_points[0].x + st_points[1].x) / 2;
        injection_ctr.y = (st_points[0].y + st_points[1].y) / 2;
    }
    if (Cover_LP.k == 0) // 求出与VC板底部轮廓线相垂直的直线方程斜率
    {
        Injection_LP.k = 100000;
    }
    else
    {
        Injection_LP.k = -1 / Cover_LP.k;
    }
    Mask_injection_ctr.x = (Mask_Rcnn_Pt1.x+Mask_Rcnn_Pt2.x)/2;
    Mask_injection_ctr.y = (Mask_Rcnn_Pt1.y+Mask_Rcnn_Pt2.y)/2;
    Injection_LP.b = Mask_injection_ctr.y - Injection_LP.k * Mask_injection_ctr.x; // 求出注液口侧边轮廓线方程的偏置
    circle(detection_result_RGB,injection_ctr,10,Scalar(255,0,0),-1); // 画出最小外接矩形注检测中心点
    circle(detection_result_RGB,Mask_injection_ctr,10,Scalar(0,0,255),-1); // 画出Mask_RCNN检测中心点
    line(detection_result_RGB,injection_ctr,Mask_injection_ctr,Scalar(0,255,0),3); // 画出注液口中垂线
}
