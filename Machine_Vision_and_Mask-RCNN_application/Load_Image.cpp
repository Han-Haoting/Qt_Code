#include "Load_Image.h"

bool Load_Image()
{
    extern Mat image;
    QFile file("E:/QT/Practice/2.bmp"); // 读入图像
    if(!file.open(QFile::ReadOnly)) // 读取失败
    {
        //ui->Result_Program_Running_Status->append(QString("图像打开失败..."));
        return 0;
    }
    QByteArray ba = file.readAll();
    image = imdecode(std::vector<char>(ba.begin(), ba.end()), 1); // 再使用imdecode函数将比特流解码成Mat类
    return 1;
}
