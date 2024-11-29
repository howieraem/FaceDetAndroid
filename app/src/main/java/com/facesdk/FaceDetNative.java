package com.facesdk;

public class FaceDetNative {
    /**
     * 初始化人脸检测SDK
     * @param faceDetModelPath 人脸检测模型权重的绝对路径，String类型
     * @param numThreads 使用的线程数量，推荐4个线程，int类型
     * @return 若成功true，否则false
     */
    public native boolean init(String faceDetModelPath, int numThreads);

    /**
     * 人脸检测函数，只检测一个人脸
     * @param imageData jpeg格式的图片（帧）数据，byte[]类型
     * @param imageWidth 图片（帧）的宽，int类型
     * @param imageHeight 图片（帧）的高，int类型
     * @param imageChannel 图片（帧）的通道数，一般为ARGB即4个通道，int类型
     * @return 若检测到人脸返回单个人脸检测框的坐标信息int[] {x_左上角，y_左上角，x_右下角，y_右下角}，否则null
     */
    public native int[] detect(byte[] imageData, int imageWidth , int imageHeight, int imageChannel);

    /**
     * 程序退出时需运行的人脸检测SDK销毁函数
     */
    public native void clean();

    /* 加载动态库 */
    static {
        System.loadLibrary("MNN");
        System.loadLibrary("MNN_CL");
        System.loadLibrary("c++_shared");
        System.loadLibrary("facedet");
    }
}
