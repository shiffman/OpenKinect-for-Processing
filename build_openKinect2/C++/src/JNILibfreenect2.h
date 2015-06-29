/*
 *  JNILibfreenect2.h
 *  JNILibfreenect2
 *
 *  Created by tom on 6/7/15.
 *  Copyright (c) 2015 tom. All rights reserved.
 *
 */

#ifndef JNI_LIB_FREENECT2_H_
#define JNI_LIB_FREENECT2_H_

#include <iostream>
#include <thread>
#include <mutex>
#include <cmath>

#include <iostream>
#include <signal.h>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/threading.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>


/* The classes below are exported */
#pragma GCC visibility push(default)

#define FRAME_SIZE_DEPTH    217088
#define FRAME_SIZE_COLOR    2073600
#define FLT_EPSILON         1.19209290e-07F


namespace  openKinect2{
    
    class Device
    {
    public:
        Device();
        
        void    open(int mode = 1);
        
        void    closeKinect();
        
        void    sigint_handler(int s);
        
        std::string getVersion();
        
        
        uint32_t *	JNI_GetDepth();
        uint32_t *  JNI_GetColor();
        uint32_t *  JNI_GetIr();
        uint32_t *  JNI_GetUndistorted();
        uint32_t *  JNI_GetRegistered();
        
        bool        isKinectReady();
    
        
        
        float valR;
        float valG;
        float valB;
        float valA;
        float gamma;
        
        float min;
        float max;
        
        bool mapDepth;
        
    private:
        void    updateKinect();
        
        void    setupDepth();
        
        int     openKinect(int mode = 1);
        
        float   clamp(float value, float min, float max);
        float   lmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp);
        
        static inline uint8_t u8fromfloat_trick(float x)
        {
            union { float f; uint32_t i; } u;
            u.f = 32768.0f + x * (255.0f / 256.0f);
            return (uint8_t)u.i;
        }
        
        //help functions
       int colorByte2Int(int gray);
       int colorByte2Int(int gray, int alpha);
        uint32_t colorByte2Int(int r, int g, int b, int  a);
        
    private:
        
        
        std::thread         mKinectThread;
        
        //initilized
        bool                initialized_device;
    
        libfreenect2::Freenect2                  freenect2;
        libfreenect2::SyncMultiFrameListener *   listener = 0;
        libfreenect2::Freenect2Device *          dev = 0;
        libfreenect2::PacketPipeline  *          pipeline = 0;
        libfreenect2::Registration    *          registration = 0;
        
        uint32_t *	 depthData;
        uint32_t *	 colorData;
        uint32_t *	 irData;
        uint32_t *   undisortedData;
        uint32_t *   registeredData;
        
        
        std::string version;
        
        std::string mSerialKinect;
        
    
    };

}
#pragma GCC visibility pop
#endif


