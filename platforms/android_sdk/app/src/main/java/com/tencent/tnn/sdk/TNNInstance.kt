package com.tencent.tnn.sdk

/**
 * Created on 2020/8/17
 * @author jwkuang
 */
class TNNInstance {
    private var tnnNativeInstance: Int = 0

    external fun nativeInit(protoPath: String, modelPath: String, computUnitType: Int): Long
}