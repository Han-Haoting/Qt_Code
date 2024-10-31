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
## Program execution process
Loading the UTVC image using the load_image function, and then import the UTVC image into the packaged Mask R-CNN for injection port feature detection. Next, use an image filling algorithm to eliminate holes in the injection port mask image, and finally apply the minimum bounding rectangle algorithm to solve the geometric information of the injection port mask image (positioning point and orientation angle).

Extracting the cover plate area from the UTVC image, and use image processing algorithms to remove isolated pixels and hole areas in the cover plate region image. Then, apply the minimum bounding rectangle algorithm to solve for the positioning point and orientation angle of the cover plate area.

Using the obtained injection port positioning point and cover plate area positioning point, construct a direction vector and calculate the angle of the direction vector as the orientation angle of the UTVC image. This improves the problem of directionality in solving the image rotation angle for the minimum bounding rectangle.
## Program execution result

