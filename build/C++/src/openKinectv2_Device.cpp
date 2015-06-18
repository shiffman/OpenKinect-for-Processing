#include "openKinectv2_Device.h"


JNIEXPORT void JNICALL Java_openKinectv2_Device_openJNI(JNIEnv* env, jobject obj)
{
    std::cout<<"init kinect 2.1"<<std::endl;
    
    openKinect2::Device * kinect = new openKinect2::Device();
    
    kinect->open(3);
    
    jclass cls = env->FindClass("openKinectv2/Device");
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    env->SetLongField(obj, fid, (jlong)kinect);
    env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_openKinectv2_Device_stopJNI(JNIEnv* env, jobject obj)
{
    jclass cls = env->FindClass("openKinectv2/Device");
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->stop();
    env->DeleteLocalRef(cls);
}


JNIEXPORT jintArray JNICALL Java_openKinectv2_Device_jniGetDepthData(JNIEnv * env, jobject obj)
{
    jclass cls = env->FindClass("openKinectv2/Device");
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    
    jintArray buffer = env->NewIntArray((jsize)FRAME_SIZE_DEPTH);
    const jint * pInt = (const jint *)kinect->JNI_GetDepth();
    env->SetIntArrayRegion(buffer, 0, (jsize)FRAME_SIZE_DEPTH, (const jint *)(pInt));
    env->DeleteLocalRef(cls);
    
    return buffer;
}
