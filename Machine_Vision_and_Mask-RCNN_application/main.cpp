#include "mainwindow.h"
#include <QtQml/qqmlapplicationengine.h>
#include <QApplication>
#include "Mask_RCNN.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    LinePara LP1;
//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    if (engine.rootObjects().isEmpty())
//        return -1;
    return a.exec();
}
