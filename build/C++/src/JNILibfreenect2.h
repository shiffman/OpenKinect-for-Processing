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
        
        
    private:
        void    updateKinect();
        
        void    setupDepth();
        
        int     openKinect(int mode = 1);
        

        
        
        
        //help functions
       int colorByte2Int(int gray);
        
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


