//
// Created by wangyan-pd on 2016/6/5.
//

#ifndef BLURJNI_THREEAPPROXIMATEGAUSSIANBLUR_H
#define BLURJNI_THREEAPPROXIMATEGAUSSIANBLUR_H
void threeApproximateGaussianblur(int* pix, int w, int h, int r);
void boxBlurH(int* srcPix, int* destPix, int w, int h, int radius);
void boxBlurV(int* srcPix, int* destPix, int w, int h, int radius);
void boxBlur(int* srcPix, int* destPix, int w, int h, int r);
void boxesForGauss(float sigma, int* size, int n);
#endif //BLURJNI_THREEAPPROXIMATEGAUSSIANBLUR_H
