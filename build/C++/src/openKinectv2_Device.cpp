#include "openKinectv2_Device.h"
#include "JNILibfreenect2.h"
#include <iostream>

JNIEXPORT jlong JNICALL Java_openKinectv2_Device_initJNI(JNIEnv* env, jobject obj)
{
    std::cout<<"init openKinectv2 "<<std::endl;
    
    openKinect2::Device * kinect = new openKinect2::Device();
    jclass cls = env->GetObjectClass(obj);
    jfieldID fieldID = env->GetFieldID(cls, "ptr", "J");
    env->SetLongField(obj, fieldID, (jlong)kinect);
    jlong ptr = env->GetLongField(obj, fieldID);
    return (jlong) ptr;
}

JNIEXPORT void JNICALL Java_openKinectv2_Device_openJNI(JNIEnv* env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->open(0);
    env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_openKinectv2_Device_stopJNI(JNIEnv* env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->closeKinect();
    
    
    env->DeleteLocalRef( cls );
    env->SetLongField( obj, fid, -1 );
    env->DeleteGlobalRef( obj );
}

JNIEXPORT jstring JNICALL Java_openKinectv2_Device_version(JNIEnv* env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    return env->NewStringUTF(kinect->getVersion().c_str());
}


JNIEXPORT jintArray JNICALL Java_openKinectv2_Device_jniGetDepthData(JNIEnv * env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    jintArray buffer = env->NewIntArray((jsize)FRAME_SIZE_DEPTH);
    
    const jint * pInt;
    if(kinect->isDepthReady()){
        pInt = (const jint * )kinect->JNI_GetDepth();
        env->SetIntArrayRegion(buffer, 0, (jsize)FRAME_SIZE_DEPTH, (const jint *)(pInt));
    }

    return buffer;
}
