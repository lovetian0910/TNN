//
// Created by jwkuang(况建炜) on 2020/8/17.
//

#include <jni.h>
#include <string>
#include "include/helper_jni.h"
#include "include/tnn_native_manager.h"

jlong nativeInit(JNIEnv* env, jstring protoPath, jstring modelPath, jint computUnitType){
    std::string protoContent, modelContent;
    std::string protoPathStr(jstring2string(env, protoPath));
    std::string modelPathStr(jstring2string(env, modelPath));
    auto* tnnManager = new TNNManager();
    tnnManager->init(protoContent, modelContent, TNNComputeUnitsGPU);
    return (jlong)tnnManager;
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    jclass c = env->FindClass("com/tencent/tnn/sdk/TNNInstance");
    if (c == nullptr) return JNI_ERR;

    // Register your class' native methods.
    static const JNINativeMethod methods[] = {
        {"nativeInit", "(Ljava/lang/String;Ljava/lang/String;I)J", reinterpret_cast<void*>(nativeInit)}
    };
    int rc = env->RegisterNatives(c, methods, sizeof(methods)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) return rc;

    return JNI_VERSION_1_6;
}

