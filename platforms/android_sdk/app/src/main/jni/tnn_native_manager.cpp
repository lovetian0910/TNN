//
// Created by jwkuang(况建炜) on 2020/8/17.
//

#include "include/tnn_native_manager.h"

TNN_NS::Status TNNManager::init(const std::string &proto_content, const std::string &model_content,
                      TNNComputeUnits units, const std::vector<int> nchw) {
    TNN_NS::Status status;
    TNN_NS::ModelConfig config;
    config.model_type = TNN_NS::MODEL_TYPE_TNN;
    config.params = {proto_content, model_content};

    auto net = std::make_shared<TNN_NS::TNN>();
    status   = net->Init(config);
    if (status != TNN_NS::TNN_OK) {
        LOGE("instance.net init failed %d", (int)status);
        return status;
    }
    net_ = net;
    device_type_ = TNN_NS::DEVICE_ARM;
    if (units >= TNNComputeUnitsGPU){
        device_type_      = TNN_NS::DEVICE_OPENCL;
    }
    TNN_NS::InputShapesMap shapeMap;
    shapeMap.insert(std::pair<std::string, TNN_NS::DimsVector>("input", nchw));
    TNN_NS::NetworkConfig network_config;
    network_config.device_type  = device_type_;
    auto instance = net_->CreateInst(network_config, status, shapeMap);
    if (status != TNN_NS::TNN_OK || !instance) {
        // try device_arm
        if (units >= TNNComputeUnitsGPU) {
            device_type_               = TNN_NS::DEVICE_ARM;
            network_config.device_type = TNN_NS::DEVICE_ARM;
            instance                   = net_->CreateInst(network_config, status, shapeMap);
        }
    }
    if(instance != nullptr){
        instance->SetCpuNumThreads(threadNum_);
    }
    instance_ = instance;
    return status;
}

TNNManager::TNNManager() {

}

TNNManager::~TNNManager() {

}

float *TNNManager::executeWithBitmap(int width, int height, void *sourcePixelscolor, bool reverseChannel, std::string outputName) {
    if(instance_ == nullptr){
        return nullptr;
    }
    TNN_NS::DeviceType dt = TNN_NS::DEVICE_ARM;
    TNN_NS::DimsVector target_dims = {1, 3, height, width};
    auto input_mat = std::make_shared<TNN_NS::Mat>(dt, TNN_NS::N8UC4, target_dims, sourcePixelscolor);
    TNN_NS::MatConvertParam input_convert_param;
    input_convert_param.reverse_channel = reverseChannel;
    auto status = instance_->SetInputMat(input_mat, input_convert_param);
    if (status != TNN_NS::TNN_OK) {
        LOGE("input_blob_convert.ConvertFromMatAsync Error: %s\n", status.description().c_str());
        return nullptr;
    }
    // step 2. Forward
    status = instance_->ForwardAsync(nullptr);
    if (status != TNN_NS::TNN_OK) {
        LOGE("instance.Forward Error: %s\n", status.description().c_str());
        return nullptr;
    }
    // step 3. get output mat
    TNN_NS::MatConvertParam output_convert_param;
    std::shared_ptr<TNN_NS::Mat> output_mat_scores = nullptr;
    status = instance_->GetOutputMat(output_mat_scores, output_convert_param, outputName);
    if (status != TNN_NS::TNN_OK) {
        LOGE("GetOutputMat Error: %s\n", status.description().c_str());
        return nullptr;
    }
    return (float*)output_mat_scores->GetData();
}

TNN_NS::Status TNNManager::setThreadNum(const int num) {
    threadNum_ = num;
    if(instance_ == nullptr){
        return instance_->SetCpuNumThreads(num);
    }
    return -1;
}

