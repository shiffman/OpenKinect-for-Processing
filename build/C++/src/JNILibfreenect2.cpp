//
//  JNILibfreenect2.cpp
//  JNILibfreenect2
//
//  Created by tom on 6/10/15.
//  Copyright (c) 2015 tom. All rights reserved.
//

#include <stdio.h>

#include "JNILibfreenect2.h"

namespace openKinect2 {
    
    void Device::sigint_handler(int s)
    {
        initialized_device = true;
    }
    
    Device::Device()
    {
        std::cout<<" Hello openKinect2 "<<std::endl;

    }
    
    int Device::openKinect(int mode)
    {
        if(freenect2.enumerateDevices() == 0)
        {
            std::cout << "no device connected!" << std::endl;
            return -1;
        }
        
        std::string serial = freenect2.getDefaultDeviceSerialNumber();
        
        
        if(mode == 1)
        {
            if(!pipeline)
                pipeline = new libfreenect2::CpuPacketPipeline();
        }
        else if(mode == 2)
        {
            
            if(!pipeline)
                pipeline = new libfreenect2::OpenGLPacketPipeline();
            else
                return -1;
            //std::cout << "OpenGL pipeline is not supported!" << std::endl;
            
        }
        else if(mode == 3)
        {
            
            if(!pipeline)
                pipeline = new libfreenect2::OpenCLPacketPipeline();
            
            //std::cout << "OpenCL pipeline is not supported!" << std::endl;
        }else{
            return -1;
        }
        
        
        if(pipeline)
        {
            dev = freenect2.openDevice(serial, pipeline);
        }
        else
        {
            std::string serial = "0123456789";
            dev = freenect2.openDevice(serial);
        }
        
        initialized_device = false;
        
       // signal(SIGINT, &Device::sigint_handler);
        listener = new libfreenect2::SyncMultiFrameListener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
        
        libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4);
        

        
        dev->setColorFrameListener(listener);
        dev->setIrAndDepthFrameListener(listener);
        dev->start();
        
        std::cout <<"setup depth"<<std::endl;
        setupDepth();
        
        std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
        std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;
        
        registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
        
        if(registration != NULL)
            return 1;
        else
            return -1;
        
        return 1;
    }
    
    void Device::setupDepth()
    {
        depthData         = (uint32_t *)malloc(FRAME_SIZE_DEPTH * sizeof(uint32_t));
    }
    
    void Device::closeKinect()
    {
        dev->stop();
        dev->close();
        
        if( registration != NULL)
            delete registration;
    }
    
    void Device::open(int mode)
    {
        int retVal = openKinect(mode);
        
        if(retVal == 1){
             mKinectThread = std::thread(&Device::updateKinect, this);
            initialized_device = true;
        }else{
            initialized_device = false;
        }
    }
    
    void Device::updateKinect()
    {
        libfreenect2::FrameMap frames;
        
        while(initialized_device){
            listener->waitForNewFrame(frames);
            
            libfreenect2::Frame * rgb   = frames[libfreenect2::Frame::Color];
            libfreenect2::Frame * ir    = frames[libfreenect2::Frame::Ir];
            libfreenect2::Frame * depth = frames[libfreenect2::Frame::Depth];
            
           // depthData[depthIndex] = colorByte2Int((uint32_t)intensity);
            
           
            
            
            //depth -> 32 bit floating point signed depth in one channel
            // 32FC1
            //cv::imshow("depth", cv::Mat(depth->height, depth->width, CV_32FC1, depth->data) / 4500.0f);
            //copy frame data the the depthData
             memcpy(depth->data, depthData, FRAME_SIZE_DEPTH * sizeof(uint32_t));
            
            
           // registration->apply(rgb,depth, &undistorted, &registered);
            
            //int key = cv::waitKey(1);
            //protonect_shutdown = protonect_shutdown || (key > 0 && ((key & 0xFF) == 27)); // shutdown on escape
            
            listener->release(frames);
        }
    }
    
    
    
    void Device::stop()
    {
       // protonect->closeKinect();
        std::cout<<"stopging kinect v2 "<<std::endl;
        
    }
    
    int Device::colorByte2Int(int gray){
        gray = gray & 0xffff;
        return 0xff000000 | (gray << 16) | (gray << 8) | gray;
    }
    
    
    uint32_t * Device::JNI_GetDepth()
    {
        return depthData;
    }
}