//
// Created by jwkuang(况建炜) on 2020/8/17.
//

#ifndef TNNSDK_TNN_NATIVE_MANAGER_H
#define TNNSDK_TNN_NATIVE_MANAGER_H
#include <string>
#include "tnn/core/macro.h"
#include "tnn/core/tnn.h"
#include "tnn/utils/blob_converter.h"
typedef enum {
    // run on cpu
    TNNComputeUnitsCPU = 0,
    // run on gpu, if failed run on cpu
    TNNComputeUnitsGPU = 1,
    // run on npu, if failed run on cpu
    TNNComputeUnitsNPU = 2,
} TNNComputeUnits;
class TNNManager{
public:
    void init(const std::string &proto_content, const std::string &model_content, TNNComputeUnits units);
    TNNManager();
    ~TNNManager();
private:
    std::shared_ptr<TNN_NS::TNN> net_           = nullptr;
    std::shared_ptr<TNN_NS::Instance> instance_ = nullptr;
};
#endif //TNNSDK_TNN_NATIVE_MANAGER_H
