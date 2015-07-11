/*
 *  JNILibfreenect2.h
 *  JNILibfreenect2
 *
 *  Created by tom on 6/7/15.
 *  Copyright (c) 2015 Thomas Sanchez Lengeling. All rights reserved.
 *
 */

#ifndef JNI_LIB_FREENECT2_H_
#define JNI_LIB_FREENECT2_H_

#include <iostream>
#include <thread>
#include <mutex>
#include <cmath>

#include <algorithm>

#include <iostream>
#include <signal.h>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/threading.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>


/* The classes below are exported */
#pragma GCC visibility push(default)


#define FRAME_SIZE_DEPTH            217088      //512 x 424
#define FRAME_BYTE_SIZE_DEPTH       868352      //512 x 424 x 4

#define FRAME_SIZE_COLOR    2073600
#define FLT_EPSILON         1.19209290e-07F


namespace  openKinect2{
    
    class Device
    {
    public:
        Device();
        
        
        //open a Device from a specific Serial Number
        void        openKinect(int index = 0);
        
        void        enumerateDevices();
        
        void        updateKinect();
        
        void        setupDepth();
        
        void        closeKinect();
        
        void        sigint_handler(int s);
        
        bool        isKinectReady();
        
        int         getDeviceCount();
        
        std::string getSerial(int index = 0);
        
        
        //get Depth Frame
        uint32_t *	JNI_GetDepth();
        
        //get color RGB  frame
        uint32_t *  JNI_GetColor();
        
        // get the IR frame
        uint32_t *  JNI_GetIr();
        
        //get the depth undistorted for the depth mapping
        uint32_t *  JNI_GetUndistorted();
        
        //get depth + rgb map
        uint32_t *  JNI_GetRegistered();
        
        //get raw depth data
        uint32_t *  JNI_GetRawDepth();
        
        float    *  JNI_GetDepthCameraPositions();
        
        void        JNI_printCameraValues();
        
        //JNI enables
        void        JNI_enableVideo(bool enable = true){enableVideo = enable;}
        void        JNI_enableDepth(bool enable = true){enableDepth = enable;}
        void        JNI_enableIR(bool enable = true){enableIR = enable;}
        void        JNI_enableRegistered(bool enable = true){enableRegistered = enable;}
        
        //toggle
        void        JNI_toggleVideo(bool enable = true){toggleVideo = enable;}
        void        JNI_toggleDepth(bool enable = true){toggleDepth = enable;}
        void        JNI_toggleIR(bool enable = true){toggleIR = enable;}
        void        JNI_toggleRegistered(bool enable = true){toggleRegistered = enable;}
        
    private:
        
        float       clamp(float value, float min, float max);
        float       lmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp);
        
        //help function to map to processing color format
        uint32_t colorByte2Int(uint32_t gray);
        uint32_t colorByte2Int(uint8_t gray, uint8_t alpha);
        uint32_t colorByte2Int(uint8_t r, uint8_t g, uint8_t b, uint8_t  a);
        uint32_t colorByte2Int(uint8_t r, uint8_t g, uint8_t b);
        
        //converssion based on
        // https://github.com/OpenKinect/libfreenect2/issues/41
        //return float pointer with x, y, and z values
        float  *     depthToCameraSpace(int & x, int & y, float & depth);
        
        
    private:
    
        //libfreenect
        libfreenect2::Freenect2                  freenect2;
        libfreenect2::SyncMultiFrameListener *   listener = 0;
        libfreenect2::Freenect2Device *          dev = 0;
        libfreenect2::PacketPipeline  *          pipeline = 0;
        libfreenect2::Registration    *          registration = 0;
        
        //Main thread
        std::thread                 mKinectThread;
        
        //initilized
        bool                        initialized_device;
        
        //Data
        uint32_t *	 depthData;
        uint32_t *   rawDepthData;
        uint32_t *	 colorData;
        uint32_t *	 irData;
        uint32_t *   undisortedData;
        uint32_t *   registeredData;
        
        float    *   depthCameraData;
        
        
        //Enables
        bool         enableDepth;
        bool         enableVideo;
        bool         enableIR;
        bool         enableRegistered;
        
        //toggles
        bool         toggleDepth;
        bool         toggleVideo;
        bool         toggleIR;
        bool         toggleRegistered;
        
        
        //for multiples Kinects
        std::string     mSerialKinect;
        
        int             mNumDevices;
        
    
    };

}
#pragma GCC visibility pop
#endif


