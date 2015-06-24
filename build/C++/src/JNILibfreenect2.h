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
        bool        isDepthReady();
    
        
        
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
        
        //float clamp(float val, float low1, float high1, float low2, float hgih2);
        
        
  
        
        //help functions
       int colorByte2Int(int gray);
       int colorByte2Int(int gray, int alpha);
       uint32_t colorByte2Int(unsigned char  a, unsigned char  r,  unsigned char  g, unsigned char  b);
        
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
        
        std::string version;
        
        std::string mSerialKinect;
        
    
    };

}
#pragma GCC visibility pop
#endif


