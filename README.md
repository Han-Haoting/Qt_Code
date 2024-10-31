# Qt_Code

## Program Development Environment
WIN10<br>
CUDA 12.4  cudatoolkit 11.3.1<br>
onnxruntime 1.8.0  onnxruntime 1.12.0<br>
opencv-python 4.6.0.66<br>
pyqt5 5.15.6<br>
pytorch 1.11.0

## Project Description
The Machine_Vision_and_Mask-RCNN_application folder contains the complete code for the project. For more details, please refer to the README file in the corresponding folder.<br>
onnxruntime.zip contains the corresponding ONNX Runtime files (such as dynamic link libraries), which need to be placed in the project folder to avoid errors during the Qt runtime due to the inability to find ONNX Runtime.<br>
Mask_RCNN_ONNX.zip contains the .py code for converting the trained Mask R-CNN model into an .onnx file, a .py file for validating the ONNX model, and the .onnx file that I personally trained and converted.<br>
Please modify the various paths involved in the program code according to your actual situation as needed.
