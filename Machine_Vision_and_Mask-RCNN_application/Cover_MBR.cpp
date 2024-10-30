#include "Cover_MBR.h"

void Cover_MBR()
{
    extern Mat cover,cover_edge,cover_RGB;
    extern Point2f cover_ctr,cover_p1,cover_p2,cover_p3,cover_p4,P1;
    extern LinePara Cover_LP,Cover_LP1;
    vector<vector<Point>> cover_contours;
    vector<Vec4i> cover_hierarchy;
    findContours(cover_edge, cover_contours, cover_hierarchy, 0, 2, Point()); //寻找轮廓的外接矩形
    RotatedRect cover_rrect;
    cover_rrect = minAreaRect(cover_contours[0]); //最小外接矩形
    Point2f cover_points[4];
    cover_rrect.points(cover_points); //读取最小外接矩形的4个顶点
    cover_ctr = cover_rrect.center; //最小外接矩形的中心
    if (cover_rrect.size.width / cover_rrect.size.height < 1) // 做最小外接矩形的长宽判断
    {
        cover_p1 = cover_points[0];
        cover_p2 = cover_points[3];
        cover_p3 = cover_points[1];
        cover_p4 = cover_points[2];
    }
    if (cover_rrect.size.width / cover_rrect.size.height > 1)
    {
        cover_p1 = cover_points[0];
        cover_p2 = cover_points[1];
        cover_p3 = cover_points[2];
        cover_p4 = cover_points[3];
    }
    GetLinePara(cover_p1.x, cover_p1.y, cover_p2.x, cover_p2.y, Cover_LP); // 求解盖板区域的底部轮廓线
    GetLinePara(cover_p2.x, cover_p2.y, cover_p3.x, cover_p3.y, Cover_LP1); // 求解盖板区域的侧边轮廓线
    cvtColor(cover,cover_RGB,COLOR_GRAY2BGR); // 转换成三通道图像
    circle(cover_RGB,cover_ctr,10,Scalar(0,0,255),-1); // 画出中心点
    P1.x = (cover_p2.x + cover_p3.x) / 2;
    P1.y = (cover_p2.y + cover_p3.y) / 2;
    line(cover_RGB, cover_p1, cover_p2, Scalar(0, 0, 255), 3);  // 画出底部轮廓线方程
    line(cover_RGB, cover_p2, cover_p3, Scalar(0, 0, 255), 3);	// 画出侧边轮廓线方程
    line(cover_RGB,cover_ctr,P1,Scalar(0,0,255),3); // 画出过中心点的底部轮廓线方程
    Cover_LP.b = cover_ctr.y - Cover_LP.k * cover_ctr.x;
}
