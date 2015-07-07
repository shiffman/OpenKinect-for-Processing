#include "org_openkinect_freenect2_Device.h"
#include "JNILibfreenect2.h"
#include <iostream>


JNIEXPORT jlong JNICALL Java_org_openkinect_freenect2_Device_jniInit(JNIEnv* env, jobject obj)
{
    std::cout<<"Init Kinect2 "<<std::endl;
    
    openKinect2::Device * kinect = new openKinect2::Device();
    jclass cls = env->GetObjectClass(obj);
    jfieldID fieldID = env->GetFieldID(cls, "ptr", "J");
    env->SetLongField(obj, fieldID, (jlong)kinect);
    jlong ptr = env->GetLongField(obj, fieldID);
    return (jlong) ptr;
}

JNIEXPORT void JNICALL Java_org_openkinect_freenect2_Device_jniOpen(JNIEnv* env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->openKinect(); //1
    env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_org_openkinect_freenect2_Device_jniOpenM(JNIEnv* env, jobject obj, jint index)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->openKinect((int)index);
    env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_org_openkinect_freenect2_Device_jniStop(JNIEnv* env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->closeKinect();
    
    //clean up the class pointer
    env->DeleteLocalRef( cls );
    env->SetLongField( obj, fid, -1 );
    env->DeleteGlobalRef( obj );
}

JNIEXPORT void JNICALL Java_org_openkinect_freenect2_Device_jniEumerateDevices(JNIEnv* env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->enumerateDevices();
    env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_org_openkinect_freenect2_Device_jniOpenSerial(JNIEnv *env, jobject obj, jint index)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->openKinect((int)index); //1
    env->DeleteLocalRef(cls);
}

JNIEXPORT int JNICALL Java_org_openkinect_freenect2_Device_jniGetNumDevices(JNIEnv * env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    return kinect->getDeviceCount();
}

JNIEXPORT jstring JNICALL Java_org_openkinect_freenect2_Device_jniGetSerialDevice(JNIEnv * env , jobject obj, jint index)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    return env->NewStringUTF(kinect->getSerial(index).c_str());
}

/// ------ get Depth data
JNIEXPORT jintArray JNICALL Java_org_openkinect_freenect2_Device_jniGetDepthData(JNIEnv * env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    jintArray buffer = env->NewIntArray((jsize)FRAME_SIZE_DEPTH);
    
    const jint * pInt;
    if(kinect->isKinectReady()){
        pInt = (const jint * )kinect->JNI_GetDepth();
        env->SetIntArrayRegion(buffer, 0, (jsize)FRAME_SIZE_DEPTH, (const jint *)(pInt));
    }

    return buffer;
}

JNIEXPORT jintArray JNICALL Java_org_openkinect_freenect2_Device_jniGetRawDepthData(JNIEnv * env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    jintArray buffer = env->NewIntArray((jsize)FRAME_SIZE_DEPTH);
    
    const jint * pInt;
    if(kinect->isKinectReady()){
        pInt = (const jint * )kinect->JNI_GetRawDepth();
        env->SetIntArrayRegion(buffer, 0, (jsize)FRAME_SIZE_DEPTH, (const jint *)(pInt));
    }
    
    return buffer;
}
/// ------  get Ir data
JNIEXPORT jintArray JNICALL Java_org_openkinect_freenect2_Device_jniGetIrData(JNIEnv * env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    jintArray buffer = env->NewIntArray((jsize)FRAME_SIZE_DEPTH);
    
    const jint * pInt;
    if(kinect->isKinectReady()){
        pInt = (const jint * )kinect->JNI_GetIr();
        env->SetIntArrayRegion(buffer, 0, (jsize)FRAME_SIZE_DEPTH, (const jint *)(pInt));
    }
    
    return buffer;
}

/// ------  get Color data
JNIEXPORT jintArray JNICALL Java_org_openkinect_freenect2_Device_jniGetColorData(JNIEnv * env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    jintArray buffer = env->NewIntArray((jsize)FRAME_SIZE_COLOR);
    
    const jint * pInt;
    if(kinect->isKinectReady()){
        pInt = (const jint * )kinect->JNI_GetColor();
        env->SetIntArrayRegion(buffer, 0, (jsize)FRAME_SIZE_COLOR, (const jint *)(pInt));
    }
    
    return buffer;
}

/*
 * Class:     org_openkinect_freenect2_Device
 * Method:    jniGetDepthData
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_org_openkinect_freenect2_Device_jniGetUndistorted(JNIEnv * env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    jintArray buffer = env->NewIntArray((jsize)FRAME_SIZE_DEPTH);
    
    const jint * pInt;
    if(kinect->isKinectReady()){
        pInt = (const jint * )kinect->JNI_GetUndistorted();
        env->SetIntArrayRegion(buffer, 0, (jsize)FRAME_SIZE_DEPTH, (const jint *)(pInt));
    }
    
    return buffer;
}

JNIEXPORT jintArray JNICALL Java_org_openkinect_freenect2_Device_jniGetRegistered(JNIEnv * env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    jintArray buffer = env->NewIntArray((jsize)FRAME_SIZE_DEPTH);
    
    const jint * pInt;
    if(kinect->isKinectReady()){
        pInt = (const jint * )kinect->JNI_GetRegistered();
        env->SetIntArrayRegion(buffer, 0, (jsize)FRAME_SIZE_DEPTH, (const jint *)(pInt));
    }
    
    return buffer;
}
