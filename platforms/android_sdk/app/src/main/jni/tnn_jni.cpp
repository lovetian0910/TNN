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
        std::string protoPathStr(jstring2string(env, proto_path));
        std::string modelPathStr(jstring2string(env, model_path));
        protoContent = fdLoadFile(protoPathStr);
        modelContent = fdLoadFile(modelPathStr);
        std::vector<int> inputDimsVector = jintArray2vector(env, input_dims);
        auto *tnnManager = new TNNManager();
        tnnManager->init(protoContent, modelContent, TNNComputeUnitsGPU, inputDimsVector);
        return (jlong) tnnManager;
    }


    JNIEXPORT jfloatArray JNICALL
    Java_com_tencent_tnn_sdk_TNNInstance_nativeExecuteWithBitmap(JNIEnv *env, jobject thiz,
                                                                 jlong native_instance, jint width,
                                                                 jint height, jobject image_source,
                                                                 jboolean reverse_channel,
                                                                 jint output_size,
                                                                 jstring output_name) {
        if(native_instance == 0){
            return nullptr;
        }
        auto* tnnManager = (TNNManager*) native_instance;
        AndroidBitmapInfo  sourceInfocolor;
        void* sourcePixelscolor;
        if (AndroidBitmap_getInfo(env, image_source, &sourceInfocolor) < 0) {
            return nullptr;
        }

        if (sourceInfocolor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
            return nullptr;
        }

        if ( AndroidBitmap_lockPixels(env, image_source, &sourcePixelscolor) < 0) {
            return nullptr;
        }
        std::string outputNameStr = jstring2string(env, output_name);
        float* retArray = tnnManager->executeWithBitmap(width, height, sourcePixelscolor, reverse_channel == JNI_TRUE, outputNameStr);
        if(retArray != nullptr){
            jfloatArray jArray = env->NewFloatArray(output_size);
            env->SetFloatArrayRegion(jArray, 0, output_size, retArray);
            return jArray;
        }
        return nullptr;
    }
}
