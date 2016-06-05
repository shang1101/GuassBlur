package com.shang.blurjni.blur;

import android.graphics.Bitmap;

/**
 * Created by wangyan-pd on 2016/6/2.
 */
public class BlurUtil {
    public native String getUtilNameFromNative();
    public native void gaussianBlur(int[] pix, int w ,int h, int r);
    public native void threeApproximateGaussianblur(int[] pix, int w ,int h, int r);
    protected static native void blurPixels(int[] img, int w, int h, int r);
    protected static native void blurBitmap(Bitmap bitmap, int r);
    static {
        System.loadLibrary("blurname");
    }
}
