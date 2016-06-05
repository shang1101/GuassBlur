//
// Created by wangyan-pd on 2016/6/5.
//

#include "ThreeApproximateGaussianblur.h"
#include "com_shang_blurjni_blur_BlurUtil.h"
#include <math.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <android/log.h>
#define  LOG_TAG    "shang_blur"
#define LOGD(...)  __android_log_print(1,ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
JNIEXPORT void JNICALL Java_com_shang_blurjni_blur_BlurUtil_threeApproximateGaussianblur
(JNIEnv * envi, jobject object, jintArray pixs, jint w, jint h, jint radius){
    int * pix = (*envi)->GetIntArrayElements(envi,pixs,NULL);
    if (pix == NULL)
        return;
    threeApproximateGaussianblur(pix,w,h,radius);
}
///////////////////////////////////////////////////////////
//利用3个均值模糊 拟合 高斯模糊
//参考：http://blog.ivank.net/fastest-gaussian-blur.html
//横向的均值模糊 srcPix：原始的像素值 destPix将处理过的像素值放入到 destPix中
void threeApproximateGaussianblur(int* pix, int w, int h, int r)
{
    float sigma = 1.0 * r / 2.57;	//2.57 *sigam半径之后基本没有贡献 所以取sigma为 r / 2.57

    int boxSize = 3;
    int* boxR = (int*)malloc(sizeof(int) * boxSize);	//需要的个数

    //计算拟合的半径
    boxesForGauss(sigma, boxR, boxSize);

    int* tempPix = (int*)malloc(sizeof(int) * w * h);

    boxBlur(pix, tempPix, w, h, (boxR[0] - 1) / 2);
    boxBlur(pix, tempPix, w, h, (boxR[1] - 1) / 2);
    boxBlur(pix, tempPix, w, h, (boxR[2] - 1) / 2);

    //清理内存
    free(boxR);
    free(tempPix);
}
void boxBlurH(int* srcPix, int* destPix, int w, int h, int radius)
{
    //用于索引
    int index;

    //r g b在遍历是 累加的色彩通道的总和
    int a = 0, r = 0, g = 0, b = 0;
    int ta, tr, tg, tb;	//临时变量

    //临时变量
    int color;
    int preColor;

    //用于计算权值 1 / num
    int num;
    float iarr;

    for (int i = 0; i < h; ++i)
    {
        r = 0;
        g = 0;
        b = 0;

        index = i * w;
        num = radius;

        for (int j = 0; j < radius; j++)
        {
            //累加0,radius-1的色彩的总和
            color = srcPix[index + j];
            //a += (color & 0xff000000) >> 24;
            r += (color & 0x00ff0000) >> 16;
            g += (color & 0x0000ff00) >> 8;
            b += (color & 0x000000ff);
        }

        //真正开始计算
        for (int j = 0; j <= radius; ++j)
        {
            num++;
            iarr = 1.0 / (1.0 * num);

            color = srcPix[index + j + radius];
            //a += (color & 0xff000000) >> 24;
            r += (color & 0x00ff0000) >> 16;
            g += (color & 0x0000ff00) >> 8;
            b += (color & 0x000000ff);

            //ta = (int)(1.0 * a / num);
            tr = (int)(r * iarr);
            tg = (int)(g * iarr);
            tb = (int)(b * iarr);

            destPix[index + j] = tr << 16 | tg << 8 | tb | 0xff000000;
        }

        iarr = 1.0 / (1.0 * num);
        for (int j = radius + 1; j < w - radius; ++j)
        {
            preColor = srcPix[index + j - 1 - radius];
            color = srcPix[index + j + radius];

            //a += (color & 0xff000000) >> 24 - (preColor & 0xff000000) >> 24;
            r = r + ((color & 0x00ff0000) >> 16) - ((preColor & 0x00ff0000) >> 16);
            g = g + ((color & 0x0000ff00) >> 8)  - ((preColor & 0x0000ff00) >> 8);
            b = b +  (color & 0x000000ff)        -  (preColor & 0x000000ff);

            //ta = (int)(1.0 * a / num);
            tr = (int)(r * iarr);
            tg = (int)(g * iarr);
            tb = (int)(b * iarr);

            destPix[index + j] = tr << 16 | tg << 8 | tb | 0xff000000;
        }

        for (int j = w - radius; j < w; ++j)
        {
            num--;
            iarr = 1.0 / (1.0 * num);

            preColor = srcPix[index + j - 1 - radius];

            //a -= (preColor & 0xff000000) >> 24;
            r -= (preColor & 0x00ff0000) >> 16;
            g -= (preColor & 0x0000ff00) >> 8;
            b -= (preColor & 0x000000ff);

            //ta = (int)(1.0 * a / num);
            tr = (int)(r * iarr);
            tg = (int)(g * iarr);
            tb = (int)(b * iarr);

            //
            //destPix[index + j] = (ta << 24 | tr << 16 | tg << 8 | tb);
            destPix[index + j] = tr << 16 | tg << 8 | tb | 0xff000000;
        }
    }
}


//列的均值模糊 srcPix：原始的像素值 destPix将处理过的像素值放入到 destPix中
void boxBlurV(int* srcPix, int* destPix, int w, int h, int radius)
{
    //r g b在遍历是 累加的色彩通道的总和
    int a = 0, r = 0, g = 0, b = 0;
    int ta, tr, tg, tb;	//临时变量

    //临时变量
    int color;
    int preColor;

    //用于计算权值 1 / num
    int num;
    float iarr;

    for (int i = 0; i < w; ++i)
    {
        r = 0;
        g = 0;
        b = 0;

        num = radius;

        for (int j = 0; j < radius; ++j)
        {
            color = srcPix[j*w + i];
            r += (color & 0x00ff0000) >> 16;
            g += (color & 0x0000ff00) >> 8;
            b += (color & 0x000000ff);
        }

        for (int j = 0; j <= radius; ++j)
        {
            num++;
            iarr = 1.0 / (1.0 * num);

            color = srcPix[(j + radius) * w + i];
            r += (color & 0x00ff0000) >> 16;
            g += (color & 0x0000ff00) >> 8;
            b += (color & 0x000000ff);

            tr = (int)(r * iarr);
            tg = (int)(g * iarr);
            tb = (int)(b * iarr);

            destPix[j*w + i] = tr << 16 | tg << 8 | tb | 0xff000000;
        }

        iarr = 1.0 / (1.0 * num);
        for (int j = radius + 1; j < h - radius; ++j)
        {
            preColor = srcPix[(j - radius - 1) * w + i];
            color = srcPix[(j + radius) * w + i];

            r = r + ((color & 0x00ff0000) >> 16) - ((preColor & 0x00ff0000) >> 16);
            g = g + ((color & 0x0000ff00) >> 8)  - ((preColor & 0x0000ff00) >> 8);
            b = b + (color & 0x000000ff)       - (preColor & 0x000000ff);

            tr = (int)(r * iarr);
            tg = (int)(g * iarr);
            tb = (int)(b * iarr);

            destPix[j*w + i] = tr << 16 | tg << 8 | tb | 0xff000000;
        }

        for (int j = h - radius; j < h; ++j)
        {
            num--;
            iarr = 1.0 / (1.0 * num);
            preColor = srcPix[(j - radius - 1) * w + i];

            r -= (preColor & 0x00ff0000) >> 16;
            g -= (preColor & 0x0000ff00) >> 8;
            b -= (preColor & 0x000000ff);

            tr = (int)(r * iarr);
            tg = (int)(g * iarr);
            tb = (int)(b * iarr);

            destPix[j*w + i] = tr << 16 | tg << 8 | tb | 0xff000000;
        }
    }
}

void boxBlur(int* srcPix, int* destPix, int w, int h, int r)
{
    if (r < 0)
    {
       // LOGD("boxBlur r < 0: %d", r);
        return;
    }

    boxBlurH(srcPix, destPix, w, h, r);
    boxBlurV(destPix, srcPix, w, h, r);
}

//领用n 个 box 拟合 sigma的高斯函数
//参考：http://www.csse.uwa.edu.au/~pk/research/pkpapers/FastGaussianSmoothing.pdf
void boxesForGauss(float sigma, int* size, int n)
{
    float wIdeal = sqrt(12.0 * sigma * sigma / n + 1.0);
    int wl = floor(wIdeal);

    if (0 == wl % 2)
        wl--;

    int wu = wl + 2;

    float mIdeal = (12.0 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) / (-4 * wl - 4);
    int m = round(mIdeal);

    for (int i = 0; i < n; ++i)
        size[i] = (i < m ? wl : wu);
}

