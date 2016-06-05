package com.shang.blurjni.blur;

import android.graphics.Bitmap;

/**
 * Created by wangyan-pd on 2016/6/5.
 */
public class GaussBlur {
    //分别在x轴 和 y轴方向上进行高斯模糊
    public Bitmap gaussBlur(Bitmap bitmap, int radius)
    {
        int w = bitmap.getWidth();
        int h = bitmap.getHeight();

        //生成一张新的图片
        Bitmap outBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);

        //定义一个临时数组存储原始图片的像素 值
        int[] pix = new int[w * h];

        //将图片像素值写入数组
        bitmap.getPixels(pix, 0, w, 0, 0, w, h);

        //进行模糊
         new BlurUtil().gaussianBlur(pix, w, h, radius);

        //将数据写入到 图片
        outBitmap.setPixels(pix, 0, w, 0, 0, w, h);

        //返回结果
        return outBitmap;
    }
}
