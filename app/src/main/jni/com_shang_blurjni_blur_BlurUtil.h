/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_shang_blurjni_blur_BlurUtil */

#ifndef _Included_com_shang_blurjni_blur_BlurUtil
#define _Included_com_shang_blurjni_blur_BlurUtil
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_shang_blurjni_blur_BlurUtil
 * Method:    getUtilNameFromNative
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_shang_blurjni_blur_BlurUtil_getUtilNameFromNative
  (JNIEnv *, jobject);

/*
 * Class:     com_shang_blurjni_blur_BlurUtil
 * Method:    gaussianBlur
 * Signature: ([IIII)V
 */
JNIEXPORT void JNICALL Java_com_shang_blurjni_blur_BlurUtil_gaussianBlur
  (JNIEnv *, jobject, jintArray, jint, jint, jint);

/*
 * Class:     com_shang_blurjni_blur_BlurUtil
 * Method:    threeApproximateGaussianblur
 * Signature: ([IIII)V
 */
JNIEXPORT void JNICALL Java_com_shang_blurjni_blur_BlurUtil_threeApproximateGaussianblur
  (JNIEnv *, jobject, jintArray, jint, jint, jint);

/*
 * Class:     com_shang_blurjni_blur_BlurUtil
 * Method:    blurPixels
 * Signature: ([IIII)V
 */
JNIEXPORT void JNICALL Java_com_shang_blurjni_blur_BlurUtil_blurPixels
  (JNIEnv *, jclass, jintArray, jint, jint, jint);

/*
 * Class:     com_shang_blurjni_blur_BlurUtil
 * Method:    blurBitmap
 * Signature: (Landroid/graphics/Bitmap;I)V
 */
JNIEXPORT void JNICALL Java_com_shang_blurjni_blur_BlurUtil_blurBitmap
  (JNIEnv *, jclass, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif