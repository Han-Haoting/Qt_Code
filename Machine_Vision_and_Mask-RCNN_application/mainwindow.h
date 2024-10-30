#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define _USE_MATH_DEFINES
#pragma execution_character_set("utf-8")
#include <QMainWindow>
#include <QString>
#include <QImage>
#include <QGraphicsScene>
#include <QFile>
#include "qmath.h"
#include "QFile"
#include "opencv.hpp"
#include "Mask_RCNN.h"
#include "imgproc/imgproc.hpp"
#include "imgproc/types_c.h"
#include "core/core.hpp"
#include "highgui/highgui.hpp"
#include "Load_Image.h"
#include "Injection_Port_Detection.h"
#include "Binarization.h"
#include "Extract_Region.h"
#include "Cover_Image_Full.h"
#include "Cover_MBR.h"
#include "Injection_Image_Full.h"
#include "Injection_MBR.h"
#include "Catch_Point.h"
#include "Rotation_Angle.h"
#include "Rotate_Image.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
struct LinePara
{
    float k;
    float b;
};
void GetLinePara(float& x1,float& y1,float& x2,float& y2,LinePara& LP); // 直线方程求解函数声明
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Camera_Calibration_clicked(); // 相机标定

    void on_Load_Image_clicked(); // 加载图像

    void on_Injection_Port_Detection_clicked(); // 注液口检测

    void on_Binarization_clicked(); // 二值化

    void on_Extract_Region_clicked(); // 提取区域

    void on_Cover_Image_Full_clicked(); // 盖板图像填充

    void on_Cover_Contour_Extraction_clicked(); // 盖板轮廓提取

    void on_Cover_MBR_clicked(); // 盖板轮廓最小外接矩形求解

    void on_Injection_Image_Full_clicked(); // 注液口图像填充

    void on_Injection_Contour_Extraction_clicked(); // 注液口轮廓提取

    void on_Injection_MBR_clicked(); // 注液口最小外接矩形

    void on_Catch_Point_clicked();

    void on_Rotation_Angle_clicked();

private:
    Ui::MainWindow *ui;

public:

};
#endif // MAINWINDOW_H
