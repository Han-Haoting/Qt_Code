#include "Injection_Port_Detection.h"

bool Injection_Port_Detection()
{
    extern Mat detection_result_RGB;
    extern int workpiece;
    Demo demo;
    demo.demoModel("E:/QT/Practice/2.bmp"); // 利用onnx进行推理
    QFile file("./masks_0.bmp");
    if(!file.open(QFile::ReadOnly)) // 检测失败，亮绿灯
    {
        return 0;
    }
    QByteArray ba = file.readAll();
    detection_result_RGB = imdecode(std::vector<char>(ba.begin(), ba.end()), 1); // 再使用imdecode函数将比特流解码成Mat类
    workpiece+=1;
    return 1;
}
