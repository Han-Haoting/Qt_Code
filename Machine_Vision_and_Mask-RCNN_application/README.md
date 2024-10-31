# Qt_Code (C++)
## Program description
Using Mask R-CNN to detect the injection port of the UTVC, and based on the detection results, applying image processing algorithms to locate the UTVC. Additionally, improving traditional visual algorithms with Mask R-CNN to correct the orientation of UTVC images.
## Program Tree
  ```
  Graduate_Program.pro + Graduate_Program.pro.user
  ├── Image loading program
  │  ├── Load_Image.cpp
  │  └── Load_Image.h
  ├── UTVC's injection port image information acquisition module
  │  ├── Mask R-CNN detection module
  │  │  ├── Mask R-CNN packaging module 
  │  │  │  ├── Mask_RCNN.cpp
  │  │  │  └── Mask_RCNN.h
  │  │  │  └── Demo.cpp.autosave
  │  │  ├──Injection port detection module 
  │  │  │  ├── Injection_Port_Detection.cpp
  │  │  │  └── Injection_Port_Detection.h
  │  ├── Image processing and information extraction module for injection port mask images
  │  │  ├── Image filling module
  │  │  │  ├── Injection_Image_Full.cpp
  │  │  │  └── Injection_Image_Full.h
  │  │  ├── Acquisition of injection port positioning points and deflection angle 
  │  │  │  ├── Injection_MBR.cpp
  │  │  │  └── Injection_MBR.h
  ├── UTVC's cover plate image information acquisition module
  │  ├── UTVC's cover plate image acquisition module
  │  │  ├── image acquisition module
  │  │  │  ├── Binarization.cpp
  │  │  │  └── Binarization.h
  │  │  │  └── Extract_Region.cpp
  │  │  │  └── Extract_Region.h
  │  │  ├──Injection port detection module 
  │  │  │  ├── Injection_Port_Detection.cpp
  │  │  │  └── Injection_Port_Detection.h
  │  ├── Image processing and information extraction module for injection port mask images
  │  │  ├── Image filling module
  │  │  │  ├── Cover_Image_Full.cpp
  │  │  │  └── Cover_Image_Full.h
  │  │  ├── Acquisition of injection port positioning points and deflection angle 
  │  │  │  ├── Cover_MBR.cpp
  │  │  │  └── Cover_MBR.h
  ├── UTVC positioning point and rotation angle solution module (Innovative points)
  │  ├── UTVC positioning point solution module
  │  │  ├── Catch_Point.cpp
  │  │  ├── Catch_Point.h
  │  ├── UTVC rotation angle solution module
  │  │  ├── Rotation_Angle.cpp
  │  │  ├── Rotation_Angle.h
  │  │  ├── Rotate_Image.cpp
  │  │  ├── Rotate_Image.h
  ├── Software interface design
  │  ├── main.cpp
  │  └── mainwindow.cpp
  │  └── mainwindow.h
  │  └── mainwindow.ui
```
