//
//  JNILibfreenect2_JNI.h
//  JNILibfreenect2
//
//  Created by tom on 6/10/15.
//  Copyright (c) 2015 tom. All rights reserved.
//
#include <jni.h>
#include <stdio.h>


#include "JNILibfreenect2.h"

#ifndef _Included_openKinectv2_Device
#define _Included_openKinectv2_Device
#ifdef __cplusplus
extern "C" {
#endif
    JNIEXPORT jstring JNICALL Java_openKinectv2_Device_stringFromJNI(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_openKinectv2_Device_nativePrint(JNIEnv* env, jobject obj);
    
    JNIEXPORT jintArray JNICALL Java_openKinectv2_Device_jniGetDepthData(JNIEnv * env, jobject obj);
    
    JNIEXPORT void JNICALL Java_openKinectv2_Device_openJNI(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_openKinectv2_Device_stopJNI(JNIEnv* env, jobject obj);
    
#ifdef __cplusplus
}
#endif
#endif