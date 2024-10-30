QT       += core gui qml widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Binarization.cpp \
    Catch_Point.cpp \
    Cover_Image_Full.cpp \
    Cover_MBR.cpp \
    Extract_Region.cpp \
    Injection_Image_Full.cpp \
    Injection_MBR.cpp \
    Injection_Port_Detection.cpp \
    Load_Image.cpp \
    Mask_RCNN.cpp \
    Rotate_Image.cpp \
    Rotation_Angle.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Binarization.h \
    Catch_Point.h \
    Cover_Image_Full.h \
    Cover_MBR.h \
    Extract_Region.h \
    Injection_Image_Full.h \
    Injection_MBR.h \
    Injection_Port_Detection.h \
    Load_Image.h \
    Mask_RCNN.h \
    Rotate_Image.h \
    Rotation_Angle.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += \
               E:\VS2017\onnx\onnxruntime-win-gpu-x64-1.8.0\include\
               E:\VS2017\opencv\build\include\
               E:\VS2017\opencv\build\include\opencv2\

LIBS += E:/VS2017/opencv/build/x64/vc15/lib/opencv_world410d.lib
LIBS += E:/VS2017/onnx/onnxruntime-win-gpu-x64-1.8.0/lib/onnxruntime.lib
#LIBS += -lonnxruntime -lonnxruntime_providers_cuda -lonnxruntime_providers_shared
