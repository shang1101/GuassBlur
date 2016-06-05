//
// Created by wangyan-pd on 2016/6/5.
//

#include "util_name.h"
#include "com_shang_blurjni_blur_BlurUtil.h"
JNIEXPORT jstring JNICALL Java_com_shang_blurjni_blur_BlurUtil_getUtilNameFromNative
        (JNIEnv * envi, jobject js){
    return (*envi)->NewStringUTF(envi,"你是个好人");
}
