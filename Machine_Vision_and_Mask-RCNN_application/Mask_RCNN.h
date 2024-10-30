#ifndef DEMO_H
#define DEMO_H
#pragma execution_character_set("utf-8")
#include <QObject>
#include <QDebug>
#include <QPointF>
#include <assert.h>
#include <vector>
#include "opencv.hpp"
#include "dnn.hpp"
#include <onnxruntime_cxx_api.h>
#include <onnxruntime_run_options_config_keys.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

class Demo : public QObject
{
    Q_OBJECT
public:
    explicit Demo(QObject *parent = nullptr);
    void demoModel(QString imagePath);


private:
    void printInputModel(Ort::Session *session); // 输入信息
    void printOutputModel(Ort::Session *session); // 输出信息
    void predictImage(Ort::Session *session, QString imgPath); // 预测图像
    void normalized(Mat mat, std::vector<float> &out); // 正则化图像

signals:

public slots:
};

#endif // DEMO_H
