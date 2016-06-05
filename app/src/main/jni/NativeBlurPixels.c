//
// Created by wangyan-pd on 2016/6/5.
//

#include <stddef.h>
#include <android/log.h>
#include "NativeBlurPixels.h"
#include "com_shang_blurjni_blur_BlurUtil.h"
#include "stackblur.h"
#include <jni.h>
#define TAG "stacknative"
#define LOG_D(...) __android_log_print(4,TAG ,__VA_ARGS__)
//__android_log_print(int prio, const char *tag,  const char *fmt, ...)
JNIEXPORT void JNICALL Java_com_shang_blurjni_blur_BlurUtil_blurPixels
        (JNIEnv *env, jclass obj, jintArray arrIn, jint w, jint h, jint r){
    // cpp:
    // pix = (env)->GetIntArrayElements(arrIn, 0);
    int * pixels = (*env)->GetIntArrayElements(env, arrIn, 0);
    if (pixels == NULL) {
        LOG_D("Input pixels isn't null.");
        return;
    }

    // Start
    pixels = blur_ARGB_8888(pixels, w, h, r);
    // End

    // if return:
    // int size = w * h;
    // jintArray result = env->NewIntArray(size);
    // env->SetIntArrayRegion(result, 0, size, pix);
    // cpp:
    // (env)->ReleaseIntArrayElements(arrIn, pix, 0);
    (*env)->ReleaseIntArrayElements(env, arrIn, pixels, 0);
    // return result;
}