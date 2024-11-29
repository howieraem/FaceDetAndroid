#include <android/bitmap.h>
#include <android/log.h>
#include <jni.h>
#include <string>
#include <vector>
#include "UltraFace.hpp"

#define max(x, y) x > y ? x : y
#define min(x, y) x < y ? x : y

using namespace std;

static UltraFace *ultra;
bool detection_sdk_init_ok = false;

extern "C" {

JNIEXPORT jboolean JNICALL Java_com_facesdk_FaceDetNative_init(
        JNIEnv *env,
        jobject instance,
        jstring faceDetModelPath_,
        jint numThreads_
        ) {
    if (detection_sdk_init_ok) {
        return true;
    }
    jboolean tRet = false;
    if (NULL == faceDetModelPath_) {
        return tRet;
    }

    const char *faceDetectionModelPath = env->GetStringUTFChars(faceDetModelPath_, 0);
    if (NULL == faceDetectionModelPath) {
        return tRet;
    }

    string tFaceModelDir = faceDetectionModelPath;
    ultra = new UltraFace(tFaceModelDir, 320, 240, numThreads_, 0.65); // config model input

    env->ReleaseStringUTFChars(faceDetModelPath_, faceDetectionModelPath);
    detection_sdk_init_ok = true;
    return true;
}

JNIEXPORT jintArray JNICALL Java_com_facesdk_FaceDetNative_detect(
        JNIEnv *env,
        jobject instance,
        jbyteArray imageData_,
        jint imageWidth,
        jint imageHeight,
        jint imageChannel) {
    if (!detection_sdk_init_ok) {
        return NULL;
    }

    int tImageDateLen = env->GetArrayLength(imageData_);
    if (imageChannel != tImageDateLen / imageWidth / imageHeight) {
        return NULL;
    }

    jbyte *imageDate = env->GetByteArrayElements(imageData_, NULL);
    if (NULL == imageDate) {
        return NULL;
    }

    if (imageWidth < 100 || imageHeight < 100) {
        return NULL;
    }

    int OUT_SIZE = 4;
    std::vector<FaceInfo> face_info;

    //detect face
    ultra->detect((unsigned char *) imageDate, imageWidth, imageHeight, imageChannel, face_info);
    int32_t num_face = static_cast<int32_t>(face_info.size());
    if (!num_face) {
        return NULL;
    }
    int *allFaceInfo = new int[OUT_SIZE];
    allFaceInfo[0] = max(0, face_info[0].x1 - 25); //left
    allFaceInfo[1] = max(0, face_info[0].y1 - 25); //top
    allFaceInfo[2] = min(face_info[0].x2 + 25, imageWidth); //right
    allFaceInfo[3] = min(face_info[0].y2 + 25, imageHeight); //bottom

    jintArray tFaceInfo = env->NewIntArray(OUT_SIZE);
    env->SetIntArrayRegion(tFaceInfo, 0, OUT_SIZE, allFaceInfo);
    env->ReleaseByteArrayElements(imageData_, imageDate, 0);
    delete[] allFaceInfo;
    return tFaceInfo;
}

JNIEXPORT void JNICALL Java_com_facesdk_FaceDetNative_clean(JNIEnv *env, jobject instance) {
    if (!detection_sdk_init_ok) {
        return;
    }
    delete ultra;
    detection_sdk_init_ok = false;
    return;
}

}
