#include "Rotation_Angle.h"

void Rotation_Angle()
{
    extern double angle;
    extern Point2f cross_pt,injection_ctr;
    angle = atan2((cross_pt.y-injection_ctr.y),(cross_pt.x-injection_ctr.x));
    angle = (angle*180/M_PI);
    // 根据求解的角度进行象限判断
    if (angle > 90 && angle < 180) // 角度在该范围内 注液口朝向第一象限
    {
        angle = angle - 90;
    }
    else if (angle > 0 && angle < 90) // 角度在该范围内 注液口朝向第二象限
    {
        angle = angle - 90;
    }
    else if (angle < 0 && angle > -90) // 角度在该范围内 注液口朝向第三象限
    {
        angle = angle - 90;
    }
    else // 角度在该范围内 注液口朝向第四象限
    {
        angle = angle - 90;
    }
}
