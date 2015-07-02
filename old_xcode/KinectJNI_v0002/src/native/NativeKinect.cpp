/*
 *  NativeKinect.cpp
 *  KinectJNI
 *
 *  Created by Daniel Shiffman on 11/14/10.
 *  Copyright 2010 ITP. All rights reserved.
 *
 */

#include "NativeKinect.h"

#import "shiffman_kinect_NativeKinect.h" // generated from NativeAddressBook.java

JNIEXPORT jint JNICALL Java_shiffman_kinect_NativeKinect_getNumber
(JNIEnv *env, jclass clazz)
{	
	return 66;
}

