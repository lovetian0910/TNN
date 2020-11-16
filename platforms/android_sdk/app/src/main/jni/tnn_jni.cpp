//
// Created by jwkuang(况建炜) on 2020/8/17.
//

#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include "include/helper_jni.h"
#include "include/tnn_native_manager.h"

extern "C" {

    JNIEXPORT jlong JNICALL
    Java_com_tencent_tnn_sdk_TNNInstance_nativeInit(JNIEnv *env, jobject thiz, jstring proto_path,
                                                    jstring model_path, jint comput_unit_type,
                                                    jintArray input_dims) {
        std::string protoContent, modelContent;
        LOGI("init start");
        std::string protoPathStr(jstring2string(env, proto_path));
        std::string modelPathStr(jstring2string(env, model_path));
        protoContent = fdLoadFile(protoPathStr);
        modelContent = fdLoadFile(modelPathStr);
        std::vector<int> inputDimsVector = jintArray2vector(env, input_dims);
        auto *tnnManager = new TNNManager();
        TNNComputeUnits computeUnits = TNNComputeUnitsCPU;
        if(comput_unit_type == 1){
            computeUnits = TNNComputeUnitsGPU;
        }
        tnnManager->init(protoContent, modelContent, computeUnits, inputDimsVector);
        return (jlong) tnnManager;
    }


    JNIEXPORT jfloatArray JNICALL
    Java_com_tencent_tnn_sdk_TNNInstance_nativeExecuteWithBitmap(JNIEnv *env, jobject thiz,
                                                                 jlong native_instance, jint width,
                                                                 jint height, jobject image_source,
                                                                 jboolean reverse_channel,
                                                                 jint output_size,
                                                                 jstring output_name,
                                                                 jfloatArray scale,
                                                                 jfloatArray bias) {
        if(native_instance == 0){
            LOGE("native_instance == 0");
            return nullptr;
        }
        auto* tnnManager = (TNNManager*) native_instance;
        AndroidBitmapInfo  sourceInfocolor;
        void* sourcePixelscolor;
        if (AndroidBitmap_getInfo(env, image_source, &sourceInfocolor) < 0) {
            LOGE("AndroidBitmap_getInfo < 0");
            return nullptr;
        }

        if (sourceInfocolor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
            LOGE("format != ANDROID_BITMAP_FORMAT_RGBA_8888");
            return nullptr;
        }

        if ( AndroidBitmap_lockPixels(env, image_source, &sourcePixelscolor) < 0) {
            LOGE("AndroidBitmap_lockPixels < 0");
            return nullptr;
        }
        std::string outputNameStr = jstring2string(env, output_name);
        std::vector<float> scaleVector = jfloatArray2vector(env, scale);
        std::vector<float> biasVector = jfloatArray2vector(env, bias);
        float* retArray = tnnManager->executeWithBitmap(width, height, sourcePixelscolor, reverse_channel == JNI_TRUE, outputNameStr, scaleVector, biasVector);
        if(retArray != nullptr){
            jfloatArray jArray = env->NewFloatArray(output_size);
            env->SetFloatArrayRegion(jArray, 0, output_size, retArray);
            return jArray;
        }
        LOGE("retArray == null");
        return nullptr;
    }

    JNIEXPORT void JNICALL
    Java_com_tencent_tnn_sdk_TNNInstance_nativeSetThreadNum(JNIEnv *env, jobject thiz,
                                                            jlong native_instance, jint thread_num) {
        if(native_instance == 0){
            return;
        }
        auto* tnnManager = (TNNManager*) native_instance;
        tnnManager->setThreadNum(thread_num);
    }
}


