//
// Created by tencent on 2020-04-30.
//

#ifndef ANDROID_HELPER_JNI_H
#define ANDROID_HELPER_JNI_H
#include <string>
#include <jni.h>

std::string fdLoadFile(std::string path);
char* jstring2string(JNIEnv* env, jstring jstr);
jstring string2jstring(JNIEnv* env, const char* pat);
std::vector<int> jintArray2vector(JNIEnv* env, jintArray array);
std::vector<float> jfloatArray2vector(JNIEnv* env, jfloatArray array);
#endif //ANDROID_HELPER_JNI_H
