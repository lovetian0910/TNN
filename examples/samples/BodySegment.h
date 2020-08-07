//
// Created by jwkuang(况建炜) on 2020/8/6.
//

#ifndef ANDROID_BODYSEGMENT_H
#define ANDROID_BODYSEGMENT_H
#include "TNNSDKSample.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class BodySegment : public TNN_NS::TNNSDKSample{
public:
    ~BodySegment();
    BodySegment();
    float* detect(std::shared_ptr<TNN_NS::Mat> image_mat);
};
#endif //ANDROID_BODYSEGMENT_H
