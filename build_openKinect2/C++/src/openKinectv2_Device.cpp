#include "openKinectv2_Device.h"
#include "JNILibfreenect2.h"
#include <iostream>

//--------------
//-- Help function
void GetJStringContent(JNIEnv *AEnv, jstring AStr, std::string &ARes) {
    if (!AStr) {
        ARes.clear();
        return;
    }
    
    const char *s = AEnv->GetStringUTFChars(AStr,NULL);
    ARes=s;
    AEnv->ReleaseStringUTFChars(AStr,s);
}
///--------------

JNIEXPORT jlong JNICALL Java_openKinectv2_Device_initJNI(JNIEnv* env, jobject obj)
{
    std::cout<<"Init openKinectv2 "<<std::endl;
    
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
    std::cout<<"Opening Kinect v2 "<<std::endl;
    kinect->openKinect(); //1
    env->DeleteLocalRef(cls);
}
JNIEXPORT void JNICALL Java_openKinectv2_Device_openSerialJNI(JNIEnv *env, jobject obj, jstring serial)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    
    std::string mSerial;
    GetJStringContent(env, serial, mSerial);
    
    std::cout<<"Opening Kinect v2 with Serial: "<<mSerial<<std::endl;
    kinect->openKinect(mSerial); //1
    env->DeleteLocalRef(cls);
}

JNIEXPORT void JNICALL Java_openKinectv2_Device_stopJNI(JNIEnv* env, jobject obj)
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

JNIEXPORT int JNICALL Java_openKinectv2_Device_getNumDevices(JNIEnv * env, jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    std::cout<<"opening kinect"<<std::endl;
    return kinect->getDeviceCount();
}

JNIEXPORT jstring JNICALL Java_openKinectv2_Device_getSerialDevice(JNIEnv * env , jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    jfieldID fid = env->GetFieldID(cls, "ptr", "J");
    openKinect2::Device * kinect = (openKinect2::Device *) env->GetLongField(obj, fid);
    std::cout<<"opening kinect"<<std::endl;
    return env->NewStringUTF(kinect->getSerial().c_str());
}

/// ------ get Depth data
JNIEXPORT jintArray JNICALL Java_openKinectv2_Device_jniGetDepthData(JNIEnv * env, jobject obj)
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

JNIEXPORT jintArray JNICALL Java_openKinectv2_Device_jniGetRawDepthData(JNIEnv * env, jobject obj)
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
JNIEXPORT jintArray JNICALL Java_openKinectv2_Device_jniGetIrData(JNIEnv * env, jobject obj)
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
JNIEXPORT jintArray JNICALL Java_openKinectv2_Device_jniGetColorData(JNIEnv * env, jobject obj)
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
 * Class:     openKinectv2_Device
 * Method:    jniGetDepthData
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_openKinectv2_Device_jniGetUndistorted(JNIEnv * env, jobject obj)
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

JNIEXPORT jintArray JNICALL Java_openKinectv2_Device_jniGetRegistered(JNIEnv * env, jobject obj)
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
