#include<stdio.h>

#include<jni.h>

extern "C" {
    JNIEXPORT jstring JNICALL Java_openKinectv2_Device_stringFromJNI(JNIEnv* env, jobject thiz);
    JNIEXPORT void JNICALL Java_openKinectv2_Device_nativePrint(JNIEnv* env, jobject thiz);
}

/*non-static method*/
JNIEXPORT jstring JNICALL Java_openKinectv2_Device_stringFromJNI(JNIEnv* env, jobject thiz)
{
    printf("native method called");
    
    return env->NewStringUTF("I'm JNI");
}

/*static method*/
JNIEXPORT void JNICALL Java_openKinectv2_Device_nativePrint(JNIEnv* env, jobject thiz)
{
    jclass cls = (jclass)thiz;
    jmethodID methodID = env->GetStaticMethodID(cls, "print", "()V");
    env->CallStaticVoidMethod(cls, methodID);
}
