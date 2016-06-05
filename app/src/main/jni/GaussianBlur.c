//
// Created by wangyan-pd on 2016/6/5.
//

#include "GaussianBlur.h"
#include <jni.h>
#include "com_shang_blurjni_blur_BlurUtil.h"
#include <math.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#define PI 3.1415926
JNIEXPORT void JNICALL Java_com_shang_blurjni_blur_BlurUtil_gaussianBlur
        (JNIEnv * env, jobject obj, jintArray pixs, jint w , jint h, jint radius){
    int * pix = (*env)->GetIntArrayElements(env,pixs,NULL);
    if(pix ==  NULL)
        return;
    float sigma =  1.0 * radius / 2.57;	//2.57 * sigam半径之后基本没有贡献 所以取sigma为 r / 2.57
    float deno  =  1.0 / (sigma * sqrt(2.0 * PI));
    float nume  = -1.0 / (2.0 * sigma * sigma);

    //高斯分布产生的数组
    float* gaussMatrix = (float*)malloc(sizeof(float)* (radius + radius + 1));
    float gaussSum = 0.0;
    for (int i = 0, x = -radius; x <= radius; ++x, ++i)
    {
        float g = deno * exp(1.0 * nume * x * x);

        gaussMatrix[i] = g;
        gaussSum += g;
    }

    //归1话
    int len = radius + radius + 1;
    for (int i = 0; i < len; ++i)
        gaussMatrix[i] /= gaussSum;

    //临时存储 一行的数据
    int* rowData  = (int*)malloc(w * sizeof(int));
    int* listData = (int*)malloc(h * sizeof(int));

    //x方向的模糊
    for (int y = 0; y < h; ++y)
    {
        //拷贝一行数据
        memcpy(rowData, pix + y * w, sizeof(int) * w);

        for (int x = 0; x < w; ++x)
        {
            float r = 0, g = 0, b = 0;
            gaussSum = 0;

            for (int i = -radius; i <= radius; ++i)
            {
                int k = x + i;

                if (0 <= k && k <= w)
                {
                    //得到像素点的rgb值
                    int color = rowData[k];
                    int cr = (color & 0x00ff0000) >> 16;
                    int cg = (color & 0x0000ff00) >> 8;
                    int cb = (color & 0x000000ff);

                    r += cr * gaussMatrix[i + radius];
                    g += cg * gaussMatrix[i + radius];
                    b += cb * gaussMatrix[i + radius];

                    gaussSum += gaussMatrix[i + radius];
                }
            }

            int cr = (int)(r / gaussSum);
            int cg = (int)(g / gaussSum);
            int cb = (int)(b / gaussSum);

            pix[y * w + x] = cr << 16 | cg << 8 | cb | 0xff000000;
        }
    }

    for (int x = 0; x < w; ++x)
    {
        //拷贝 一列 数据
        for (int y = 0; y < h; ++y)
            listData[y] = pix[y * w + x];

        for (int y = 0; y < h; ++y)
        {
            float r = 0, g = 0, b = 0;
            gaussSum = 0;

            for (int j = -radius; j <= radius; ++j)
            {
                int k = y + j;

                if (0 <= k && k <= h)
                {
                    int color = listData[k];
                    int cr = (color & 0x00ff0000) >> 16;
                    int cg = (color & 0x0000ff00) >> 8;
                    int cb = (color & 0x000000ff);

                    r += cr * gaussMatrix[j + radius];
                    g += cg * gaussMatrix[j + radius];
                    b += cb * gaussMatrix[j + radius];

                    gaussSum += gaussMatrix[j + radius];
                }
            }

            int cr = (int)(r / gaussSum);
            int cg = (int)(g / gaussSum);
            int cb = (int)(b / gaussSum);

            pix[y * w + x] = cr << 16 | cg << 8 | cb | 0xff000000;
        }
    }

    //清理内存
    free(gaussMatrix);
    free(rowData);
    free(listData);

}
