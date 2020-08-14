//
// Created by jwkuang(况建炜) on 2020/8/6.
//

#include "BodySegment.h"
#include <string>

BodySegment::~BodySegment() {

}

BodySegment::BodySegment() {

}

float *BodySegment::detect(std::shared_ptr<TNN_NS::Mat> image_mat) {
    if (!image_mat || !image_mat->GetData()) {
        std::cout << "image is empty ,please check!" << std::endl;
        return nullptr;
    }
    TNN_NS::MatConvertParam input_convert_param;
    input_convert_param.reverse_channel = true;
    auto status = instance_->SetInputMat(image_mat, input_convert_param);
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
    status = instance_->GetOutputMat(output_mat_scores, output_convert_param, "activation/truediv:0");
    if (status != TNN_NS::TNN_OK) {
        LOGE("GetOutputMat Error: %s\n", status.description().c_str());
        return nullptr;
    }

    return (float*)output_mat_scores->GetData();
}

