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

JNIEXPORT jint JNICALL Java_org_openkinect_freenect2_Device_jniGetNumDevices(JNIEnv * env, jobject obj)
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

JNIEXPORT jfloatArray JNICALL Java_org_openkinect_freenect2_Device_jniGetDepthCameraPositions
(JNIEnv * env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    jfloatArray buffer = env->NewFloatArray((jsize)FRAME_SIZE_DEPTH * 3);
    
    const jfloat * pFloat;
    if(kinect->isKinectReady()){
        pFloat = (const jfloat * )kinect->JNI_GetDepthCameraPositions();
        env->SetFloatArrayRegion(buffer, 0, (jsize)FRAME_SIZE_DEPTH * 3, (const jfloat *)(pFloat));
    }
    
    return buffer;
}


//----enable

JNIEXPORT void JNICALL Java_org_openkinect_freenect2_Device_jniEnableVideo
(JNIEnv * env , jobject obj, jboolean enable)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->JNI_enableVideo(bool(enable));
    env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_org_openkinect_freenect2_Device_jniEnableDepth
(JNIEnv * env , jobject obj, jboolean enable)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->JNI_enableDepth(bool(enable));
    env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_org_openkinect_freenect2_Device_jniEnableIR
(JNIEnv * env , jobject obj, jboolean enable)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->JNI_enableIR(bool(enable));
    env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_org_openkinect_freenect2_Device_jniEnableRegistered
(JNIEnv * env , jobject obj, jboolean enable)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->JNI_enableRegistered(bool(enable));
    env->DeleteLocalRef(cls);
}

//-----toggle

JNIEXPORT void JNICALL Java_org_openkinect_freenect2_Device_jniToggleVideo
(JNIEnv * env , jobject obj, jboolean toggle)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->JNI_toggleVideo(bool(toggle));
    env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_org_openkinect_freenect2_Device_jniToggleDepth
(JNIEnv * env , jobject obj, jboolean toggle)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->JNI_toggleDepth(bool(toggle));
    env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_org_openkinect_freenect2_Device_jniToggleIR
(JNIEnv * env , jobject obj, jboolean toggle)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->JNI_toggleIR(bool(toggle));
    env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_org_openkinect_freenect2_Device_jniToggleRegistered
(JNIEnv * env , jobject obj, jboolean toggle)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    kinect->JNI_toggleRegistered(bool(toggle));
    env->DeleteLocalRef(cls);
}
