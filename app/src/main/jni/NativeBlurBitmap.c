//
// Created by wangyan-pd on 2016/6/5.
//

#include "NativeBlurBitmap.h"
#include "com_shang_blurjni_blur_BlurUtil.h"
#include "stackblur.h"
#include <android/bitmap.h>
#include <android/log.h>

#define TAG "stacknative"
#define LOG_D(...) __android_log_print(4,TAG ,__VA_ARGS__)
JNIEXPORT void JNICALL Java_com_shang_blurjni_blur_BlurUtil_blurBitmap
    (JNIEnv * env,jclass obj, jobject bitmapIn,jint r) {
        AndroidBitmapInfo infoIn;
        void *pixels;

        // Get image info
        if (AndroidBitmap_getInfo(env, bitmapIn, &infoIn) != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOG_D("AndroidBitmap_getInfo failed!");
        return;
        }

        // Check image
        if (infoIn.format != ANDROID_BITMAP_FORMAT_RGBA_8888 &&
        infoIn.format != ANDROID_BITMAP_FORMAT_RGB_565) {
        LOG_D("Only support ANDROID_BITMAP_FORMAT_RGBA_8888 and ANDROID_BITMAP_FORMAT_RGB_565");
        return;
        }

        // Lock all images
        if (AndroidBitmap_lockPixels(env, bitmapIn, &pixels) != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOG_D("AndroidBitmap_lockPixels failed!");
            return;
        }
        // height width
        int h = infoIn.height;
        int w = infoIn.width;

        // Start
        if (infoIn.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            pixels = blur_ARGB_8888((int *) pixels, w, h, r);
        } else if (infoIn.format == ANDROID_BITMAP_FORMAT_RGB_565) {
            pixels = blur_RGB_565((short *) pixels, w, h, r);
        }

        // End

        // Unlocks everything
        AndroidBitmap_unlockPixels(env, bitmapIn);
}