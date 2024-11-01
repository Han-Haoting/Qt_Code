﻿#include "Mask_RCNN.h"

Demo::Demo(QObject *parent) : QObject(parent)
{

}

void Demo::demoModel(QString imagePath)
{
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "test");    /** 初始化环境，每个进程一个环境.环境保留了线程池和其他状态信息 **/

    /**
    * 初始化Session选项
    * Available levels are
    * ORT_DISABLE_ALL -> 禁用所有优化
    * ORT_ENABLE_BASIC -> 要启用基本优化(如冗余节点删除)
    * ORT_ENABLE_EXTENDED -> 启用扩展优化(包括1级以上更复杂的优化，如节点融合)
    * ORT_ENABLE_ALL -> 启用所有可能的优化
    **/
    Ort::SessionOptions session_options;
    session_options.SetIntraOpNumThreads(1);
    session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED); /** 设置图像优化级别 **/

    //*************************************************************************
    // 创建Session并把模型载入内存
#ifdef _WIN32
    const wchar_t* model_path = L"E:/QT/Practice/model.onnx";
#else
    const char* model_path = "model2.onnx";
#endif
    printf("Using Onnxruntime C++ API\n");
    Ort::Session session(env, model_path, session_options);
    //*************************************************************************

    printInputModel(&session);  //打印一下模型输入层
    qDebug("/*****************************************/");
    printOutputModel(&session); //打印一下模型输出层
    qDebug("/*****************************************/");

    predictImage(&session, imagePath);

}

void Demo::printInputModel(Ort::Session *session)
{
    Ort::AllocatorWithDefaultOptions allocator;

    //打印模型输入节点的数量
    size_t num_input_nodes = session->GetInputCount();
    std::vector<const char*> input_node_names(num_input_nodes);
    std::vector<int64_t> input_node_dims;   /** 简化……该模型只有1个输入节点{1, 3, 1024, 1278}, 否则需要 vector<vector<>> **/
    qDebug() << "输入节点数量 = " <<  num_input_nodes;

    // 遍历所有输入节点
    for (int i = 0; i < num_input_nodes; i++) {
        // 打印输入节点名称
        char* input_name = session->GetInputName(i, allocator);
        qDebug("输入节点第 %d 个: name=%s", i, input_name);
        input_node_names[i] = input_name;

        // 打印输入节点类型
        Ort::TypeInfo type_info = session->GetInputTypeInfo(i);
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
        ONNXTensorElementDataType type = tensor_info.GetElementType();
        qDebug("输入节点第 %d 个: type=%d", i, type);

        // 打印输入 shapes/dims
        input_node_dims = tensor_info.GetShape();
        qDebug("输入节点第 %d 个: num_dims=%zu", i, input_node_dims.size());
        for (int j = 0; j < input_node_dims.size(); j++)
            qDebug("输入节点第 %d 个: dim %d=%jd", i, j, input_node_dims[j]);
    }
}

void Demo::printOutputModel(Ort::Session *session)
{
    Ort::AllocatorWithDefaultOptions allocator;
    //打印模型输出节点的数量
    size_t num_output_nodes = session->GetOutputCount();
    std::vector<const char*> output_node_names(num_output_nodes);
    std::vector<int64_t> output_node_dims;
    qDebug() << "输出节点数量 = " <<  num_output_nodes;

    // 遍历所有输出节点
    for (int i = 0; i < num_output_nodes; i++) {
        // 打印输出节点名称
        char* output_name = session->GetOutputName(i, allocator);
        qDebug("输出节点第 %d 个: name=%s", i, output_name);
        output_node_names[i] = output_name;

        // 打印输出节点类型
        Ort::TypeInfo type_info = session->GetOutputTypeInfo(i);
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
        ONNXTensorElementDataType type = tensor_info.GetElementType();
        qDebug("输出节点第 %d 个: type=%d", i, type);

        // 打印输出 shapes/dims
        output_node_dims = tensor_info.GetShape();
        qDebug("输出节点第 %d 个: num_dims=%zu", i, output_node_dims.size());
        for (int j = 0; j < output_node_dims.size(); j++)
            qDebug("输出节点第 %d 个: dim %d=%jd", i, j, output_node_dims[j]);
    }
}

void Demo::predictImage(Ort::Session *session, QString imgPath)
{
    extern Point2f Mask_Rcnn_Pt1,Mask_Rcnn_Pt2; // 用于存放Mask_RCNN网络的预测框坐标
    // 预处理图片
    cv::Mat mat = cv::imread(imgPath.toStdString().data());   /** 读取需要预测的图片 **/
    cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);                  /** 将图片的通道转变一下 **/
    size_t input_tensor_size = mat.size().width * mat.size().height * 3;
    std::vector<float> input_data(input_tensor_size);
    normalized(mat, input_data);                                /** 归一化图片数据 **/

    // 从数据值创建输入张量对象
    std::vector<int64_t> input_node_dims = {1, 3, mat.size().height, mat.size().width};
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(memory_info, input_data.data(), input_tensor_size, input_node_dims.data(), input_node_dims.size());
    assert(input_tensor.IsTensor());
    std::vector<Ort::Value> ort_inputs;
    ort_inputs.push_back(std::move(input_tensor));

    // 设计输入和输出的名字
    std::vector<const char*> input_node_names = {"image"};
    std::vector<const char*> output_node_names = {"boxes", "labels", "scores", "masks"};
    // 输入图片数据，运行模型获取预测结果，
    std::vector<Ort::Value> output_tensors = session->Run(Ort::RunOptions{nullptr}, input_node_names.data(), ort_inputs.data(), 1, output_node_names.data(), 4);
    assert(output_tensors.size() == 4 && output_tensors.front().IsTensor());

    // 获取一下预测结果
    Ort::AllocatorWithDefaultOptions allocator;
    for(int i = 0; i < output_tensors.size(); i++)
    {
        char* output_name = session->GetOutputName(i, allocator);
        qDebug() << "/****************"<<output_name<<"*******************/";
        Ort::Value *output = &output_tensors[i];
        float *value = output->GetTensorMutableData<float>();
        std::vector<int64_t> shape = output->GetTypeInfo().GetTensorTypeAndShapeInfo().GetShape();
        int index = 0;
        if(strcmp(output_name, "boxes") == 0)
        {
            std::vector<std::vector<QPointF>> boxes;
            for(int x = 0; x < shape[0]; x++)
            {
                qDebug()<<value[index]<<","<<value[index+1]<<" | "<<value[index+2]<<","<<value[index+3];
                Mask_Rcnn_Pt1.x = value[index]; Mask_Rcnn_Pt1.y = value[index+1];
                Mask_Rcnn_Pt2.x = value[index+2]; Mask_Rcnn_Pt2.y = value[index+3];
                std::vector<QPointF> points;
                points.push_back(QPointF(value[index++],value[index++]));
                points.push_back(QPointF(value[index++], value[index++]));
                boxes.push_back(points);
            }
        }
        if(strcmp(output_name, "labels") == 0)
        {
            std::vector<float> labels;
            int64_t *labelV = output->GetTensorMutableData<int64_t>();
            for(int x = 0; x < shape[0]; x++)
            {
                qDebug() << labelV[x];
                labels.push_back(value[x]);
            }
        }
        if(strcmp(output_name, "scores") == 0)
        {
            std::vector<float> scores;
            for(int x = 0; x < shape[0]; x++)
            {
                qDebug() << value[x];
                scores.push_back(value[x]);
            }
        }
        if(strcmp(output_name, "masks") == 0)
        {
            std::vector<cv::Mat> masks;
            index = 0;
            for(int x = 0; x < shape[0]; x++)
            {
                cv::Mat mask(shape[2], shape[3], CV_32FC1);
                memcpy(mask.data, value + x * shape[2] * shape[3], shape[2]*shape[3] * sizeof(float));
                QString name = QString("./masks_%1.bmp").arg(x);
                cv::imwrite(name.toStdString().data(), mask*255);
                masks.push_back(mask);
            }
        }

    }

}
void Demo::normalized(cv::Mat input_tensor, std::vector<float> &output_data)
{
    std::size_t counter = 0;
    for (unsigned k = 0; k < 3; k++)
    {
        for (unsigned i = 0; i < input_tensor.rows; i++)
        {
            for (unsigned j = 0; j < input_tensor.cols; j++)
            {
                output_data[counter++]=static_cast<float>(input_tensor.at<cv::Vec3b>(i, j)[k]) / 255.0;
            }
        }
    }
}
