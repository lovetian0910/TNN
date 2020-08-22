package com.tencent.tnn.sdk;

import android.graphics.Bitmap;

/**
 * Created on 2020/8/19
 *
 * @author jwkuang
 */
public class TNNInstance {
    public static final int COMPUTE_UNIT_TYPE_CPU = 0;
    public static final int COMPUTE_UNIT_TYPE_GPU = 1;
    static {
        System.loadLibrary("tnn_android_sdk");
    }

    private long tnnNativeInstance = 0L;

    public void init(String protoPath, String modelPath, int computUnitType, int[] inputDims){
        tnnNativeInstance = nativeInit(protoPath, modelPath, computUnitType, inputDims);
    }

    public float[] executeWithBitmap(int width, int height, Bitmap imageSource, boolean reverseChannel, int outputSize, String outputName){
        if(tnnNativeInstance != 0L){
            return nativeExecuteWithBitmap(tnnNativeInstance, width, height, imageSource, reverseChannel, outputSize, outputName);
        }
        return null;
    }

    public void setThreadNum(int threadNum){
        if(tnnNativeInstance != 0L){
            nativeSetThreadNum(tnnNativeInstance, threadNum);
        }
    }
    public native long nativeInit(String protoPath, String modelPath, int computUnitType, int[] inputDims);

    public native float[] nativeExecuteWithBitmap(long nativeInstance, int width, int height, Bitmap imageSource, boolean reverseChannel, int outputSize, String outputName);

    public native void nativeSetThreadNum(long nativeInstance, int threadNum);
}
