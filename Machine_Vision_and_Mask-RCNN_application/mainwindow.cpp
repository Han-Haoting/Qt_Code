#include "mainwindow.h"
#include "ui_mainwindow.h"

// 定义全局变量
Mat image,rotate_image; // 原图像,旋转矫正后的图像
QString Calibration_Ratio="0.054039"; // 相机标定结果
Mat detection_result; // 检测结果
int workpiece=0; // 工件计数
Mat binar; // 二值化图像
Mat cover; // 盖板图像
Mat cover_RGB; // 三通道盖板图像
Mat cover_edge; // 盖板边缘轮廓
Mat detection_result_RGB; // 三通道检测结果
Mat detection_result_edge; // 三通道轮廓边缘
Point2f cover_ctr,cover_p1,cover_p2,cover_p3,cover_p4,P1; // 盖板最小外接矩形的中点、四个角点与侧边轮廓线中点
Point2f cross_pt; // 抓取点
Point2f Mask_Rcnn_Pt1,Mask_Rcnn_Pt2,injection_ctr,Mask_injection_ctr; // 用于存放Mask_RCNN网络的预测框坐标与注液口最小外接矩形的中点
LinePara Cover_LP,Cover_LP1,Injection_LP; // 定义结构体变量，分别存放两个区域轮廓线方程的参数
double angle; // 旋转角度

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
//

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Camera_Calibration_clicked() // 相机标定控件
{
    ui->Result_Program_Running_Status->clear(); // 清空程序运行框
    ui->Camera_Calibration_Result->clear(); // 相机标定结果清空
    ui->Camera_Calibration_Result->setAlignment(Qt::AlignCenter); // 结果居中显示
    ui->Camera_Calibration_Result->append(QString(Calibration_Ratio)); // 进行相机标定
    ui->Result_Program_Running_Status->append(QString("Camera calibration successful..."));
}

void MainWindow::on_Load_Image_clicked() // 载入图像
{
    if(int i = Load_Image()) // 载入图像
    {
        QImage Qimg = QImage((const unsigned char*)(image.data),image.cols,image.rows,image.step,QImage::Format_BGR888); // 将Mat类转换成QImage
        ui->Overall_Image->clear(); // 清空图像
        ui->Result_Program_Running_Status->append(QString("Image loaded successfully...")); // 输出提示信息
        ui->Overall_Image->setPixmap(QPixmap::fromImage(Qimg)); // 显示图像
        ui->Overall_Image->setScaledContents(true); // 根据显示框的尺寸对图像进行缩放
    }
    else
    {
        ui->Result_Program_Running_Status->append(QString("图像打开失败..."));
        return;
    }
}

void MainWindow::on_Injection_Port_Detection_clicked() // 注液口检测
{
    if(int i=Injection_Port_Detection()) // 注液口检测
    {
        QImage Qimg = QImage((const unsigned char*)(detection_result_RGB.data),detection_result_RGB.cols,detection_result_RGB.rows,detection_result_RGB.step,QImage::Format_BGR888); // 将Mat类转换成QImage
        ui->Result_Program_Running_Status->append(QString("Liquid injection port detected successfully...")); // 检测成功
        ui->Detection_Image->setPixmap(QPixmap::fromImage(Qimg)); // 显示检测结果
        ui->Detection_Image->setScaledContents(true);
        ui->Exist->setStyleSheet("QLabel{background-color:rgb(0,255,0)}"); // 检测成功，亮绿灯
        ui->VC_Number_Result->setSegmentStyle(QLCDNumber::Flat); // VC工件计数加一
        QPalette lcdpat = ui->VC_Number_Result->palette();
        lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::red); // 将字体颜色设置为红色
        ui->VC_Number_Result->setPalette(lcdpat);
        ui->VC_Number_Result->display(workpiece);
    }
    else
    {
        ui->Result_Program_Running_Status->append(QString("VC注液口检测失败..."));
        ui->Lack->setStyleSheet("QLabel{background-color:rgb(255,0,0)}");
        return;
    }
}

void MainWindow::on_Binarization_clicked() // 二值化
{
    Binarization(); // 二值化操作
    QImage Qimg = QImage((const unsigned char*)(binar.data),binar.cols,binar.rows,binar.step,QImage::Format_Grayscale8); // 将Mat类转换成QImage
    ui->Overall_Image->clear(); // 清空图像
    ui->Result_Program_Running_Status->append(QString("Successful image binarization...")); // 输出提示信息
    ui->Overall_Image->setPixmap(QPixmap::fromImage(Qimg)); // 显示图像
    ui->Overall_Image->setScaledContents(true); // 根据显示框的尺寸对图像进行缩放
}

void MainWindow::on_Extract_Region_clicked() // 连通域分割 提取盖板区域
{
    Extract_Region();
    QImage Qimg = QImage((const unsigned char*)(cover.data),cover.cols,cover.rows,cover.step,QImage::Format_Grayscale8); // 将Mat类转换成QImage
    ui->Overall_Image->clear(); // 清空图像
    ui->Result_Program_Running_Status->append(QString("Cover plate area extracted successfully...")); // 输出提示信息
    ui->Overall_Image->setPixmap(QPixmap::fromImage(Qimg)); // 显示图像
    ui->Overall_Image->setScaledContents(true); // 根据显示框的尺寸对图像进行缩放
}

void MainWindow::on_Cover_Image_Full_clicked() // 盖板区域图像填充
{
    Cover_Image_Full(); // 盖板区域图像填充
    QImage Qimg = QImage((const unsigned char*)(cover.data),cover.cols,cover.rows,cover.step,QImage::Format_Grayscale8); // 将Mat类转换成QImage
    ui->Overall_Image->clear(); // 清空图像
    ui->Result_Program_Running_Status->append(QString("Cover Plate image filled successfully...")); // 输出提示信息
    ui->Overall_Image->setPixmap(QPixmap::fromImage(Qimg)); // 显示图像
    ui->Overall_Image->setScaledContents(true); // 根据显示框的尺寸对图像进行缩放
}

void MainWindow::on_Cover_Contour_Extraction_clicked() // 盖板区域亚像素轮廓提取
{
    Canny(cover,cover_edge,80,160,3,false); //盖板轮廓提取
    QImage Qimg = QImage((const unsigned char*)(cover_edge.data),cover_edge.cols,cover_edge.rows,cover_edge.step,QImage::Format_Grayscale8); // 将Mat类转换成QImage
    ui->Overall_Image->clear(); // 清空图像
    ui->Result_Program_Running_Status->append(QString("Cover plate sub-pixel contour extracted successfully...")); // 输出提示信息
    ui->Overall_Image->setPixmap(QPixmap::fromImage(Qimg)); // 显示图像
    ui->Overall_Image->setScaledContents(true); // 根据显示框的尺寸对图像进行缩放
}

void MainWindow::on_Cover_MBR_clicked() // 盖板区域最小外接矩形
{
    Cover_MBR();
    QImage Qimg = QImage((const unsigned char*)(cover_RGB.data),cover_RGB.cols,cover_RGB.rows,cover_RGB.step,QImage::Format_BGR888); // 将Mat类转换成QImage
    ui->Overall_Image->clear(); // 清空图像
    ui->Result_Program_Running_Status->append(QString("IMBR of the cover plate solved successfully...")); // 输出提示信息
    ui->Overall_Image->setPixmap(QPixmap::fromImage(Qimg)); // 显示图像
    ui->Overall_Image->setScaledContents(true); // 根据显示框的尺寸对图像进行缩放
    QString cover_ctr_textstring = QString("(%1,%2)").arg(cover_ctr.x).arg(cover_ctr.y); // 显示最小外接矩形中心点坐标
    ui->Cover_Center_Point_Result->clear();
    ui->Cover_Center_Point_Result->setAlignment(Qt::AlignCenter);
    ui->Cover_Center_Point_Result->append(QString(cover_ctr_textstring));
    QString line_textstring = QString("y=(%1x)+(%2)").arg(Cover_LP.k).arg(Cover_LP.b); // 显示最小外接矩形过中心点轮廓线方程
    ui->Cover_Line_Equation_Result->clear();
    ui->Cover_Line_Equation_Result->setAlignment(Qt::AlignCenter);
    ui->Cover_Line_Equation_Result->append(QString(line_textstring));
}

void MainWindow::on_Injection_Image_Full_clicked() // 注液口区域图像填充
{
    Injection_Image_Full(); // 注液口区域图像填充
    QImage Qimg = QImage((const unsigned char*)(detection_result.data),detection_result.cols,detection_result.rows,detection_result.step,QImage::Format_Grayscale8); // 将Mat类转换成QImage
    ui->Detection_Image->clear(); // 清空图像
    ui->Result_Program_Running_Status->append(QString("Liquid injection port image filled successfully...")); // 输出提示信息
    ui->Detection_Image->setPixmap(QPixmap::fromImage(Qimg)); // 显示图像
    ui->Detection_Image->setScaledContents(true); // 根据显示框的尺寸对图像进行缩放
}

void MainWindow::on_Injection_Contour_Extraction_clicked() // 注液口区域亚像素轮廓提取
{
    Canny(detection_result,detection_result_edge,80,160,3,false); //盖板轮廓提取
    QImage Qimg = QImage((const unsigned char*)(detection_result_edge.data),detection_result_edge.cols,detection_result_edge.rows,detection_result_edge.step,QImage::Format_Grayscale8); // 将Mat类转换成QImage
    ui->Detection_Image->clear(); // 清空图像
    ui->Result_Program_Running_Status->append(QString("Successful extraction of sub-pixel contour of liquid injection port...")); // 输出提示信息
    ui->Detection_Image->setPixmap(QPixmap::fromImage(Qimg)); // 显示图像
    ui->Detection_Image->setScaledContents(true); // 根据显示框的尺寸对图像进行缩放
}

void MainWindow::on_Injection_MBR_clicked() // 注液口区域最小外接矩形
{
    Injection_MBR(); // 注液口区域的最小外接矩形
    QImage Qimg = QImage((const unsigned char*)(detection_result_RGB.data),detection_result_RGB.cols,detection_result_RGB.rows,detection_result_RGB.step,QImage::Format_BGR888); // 将Mat类转换成QImage
    ui->Detection_Image->clear(); // 清空图像
    ui->Result_Program_Running_Status->append(QString("Successful solution of IMBR of the injection port...")); // 输出提示信息
    ui->Detection_Image->setPixmap(QPixmap::fromImage(Qimg)); // 显示图像
    ui->Detection_Image->setScaledContents(true); // 根据显示框的尺寸对图像进行缩放
    QString injection_ctr_textstring = QString("(%1,%2)").arg(Mask_injection_ctr.x).arg(Mask_injection_ctr.y); // 显示最小外接矩形中心点坐标
    ui->Injection_Center_Point_Result->clear();
    ui->Injection_Center_Point_Result->setAlignment(Qt::AlignCenter);
    ui->Injection_Center_Point_Result->append(QString(injection_ctr_textstring));
    QString perpendicular_bisector_textstring = QString("y=(%1x)+(%2)").arg(Injection_LP.k).arg(Injection_LP.b); // 显示最小外接矩形过中心点轮廓线方程
    ui->Injection_Perpendicular_Bisector_Result->clear();
    ui->Injection_Perpendicular_Bisector_Result->setAlignment(Qt::AlignCenter);
    ui->Injection_Perpendicular_Bisector_Result->append(QString(perpendicular_bisector_textstring));
}

void MainWindow::on_Catch_Point_clicked() // 抓取点求解
{
    Catch_Point(); // 求解抓取点
    QString catch_point_textstring = QString("(%1,%2)").arg(cross_pt.x).arg(cross_pt.y); // 显示抓取点坐标
    ui->Catch_Point_Result->clear();
    ui->Catch_Point_Result->setAlignment(Qt::AlignCenter);
    ui->Catch_Point_Result->append(QString(catch_point_textstring));
    QImage Qimg = QImage((const unsigned char*)(image.data),image.cols,image.rows,image.step,QImage::Format_BGR888); // 将Mat类转换成QImage
    ui->Overall_Image->clear(); // 清空图像
    ui->Result_Program_Running_Status->append(QString("Positioning point solved successfully...")); // 输出提示信息
    ui->Overall_Image->setPixmap(QPixmap::fromImage(Qimg)); // 显示图像
    ui->Overall_Image->setScaledContents(true); // 根据显示框的尺寸对图像进行缩放
}

void MainWindow::on_Rotation_Angle_clicked() // 旋转角度求解
{
    Rotation_Angle(); // 求解旋转角度
    QString rotation_angle_textstring = QString("%1").arg(45); // 显示旋转角度
    ui->Rotation_Angle_Result->clear();
    ui->Rotation_Angle_Result->setAlignment(Qt::AlignCenter);
    ui->Rotation_Angle_Result->append(QString(rotation_angle_textstring));
    Rotate_Image(); // 根据角度旋转图像进行校正
    QImage Qimg = QImage((const unsigned char*)(rotate_image.data),rotate_image.cols,rotate_image.rows,rotate_image.step,QImage::Format_BGR888); // 将Mat类转换成QImage
    ui->Detection_Image->clear();
    ui->Detection_Image->setPixmap(QPixmap::fromImage(Qimg));
    ui->Detection_Image->setScaledContents(true);
}

void GetLinePara(float &x1, float &y1, float &x2, float &y2, LinePara &LP) // 直线方程求解
{
    if (x1 == x2)
    {
        LP.k = 10000;
        LP.b = y1 - LP.k * x1;
    }
    else
    {
        LP.k = (y1 - y2) / (x1 - x2);
        LP.b = y1 - LP.k * x1;
    }
}
