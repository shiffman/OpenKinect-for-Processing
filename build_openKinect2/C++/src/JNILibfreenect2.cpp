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
        std::cout<<"Hello openKinect2 "<<std::endl;
        
        initialized_device = false;
        depthData = (uint32_t *)malloc(FRAME_SIZE_DEPTH * sizeof(uint32_t));
        irData    = (uint32_t *)malloc(FRAME_SIZE_DEPTH * sizeof(uint32_t));
        colorData = (uint32_t *)malloc(FRAME_SIZE_COLOR * sizeof(uint32_t));

        
        version = "0.01";
        mSerialKinect = "";
        
        mapDepth = false;
        gamma =1.0f;
        
        min = 0;
        max = 4500;

    }
    
    std::string Device::getVersion()
    {
        return version;
    }
    
    int Device::openKinect(int mode)
    {
        if(freenect2.enumerateDevices() == 0)
        {
            std::cout << "no device connected!" << std::endl;
            initialized_device = false;
            return -1;
        }
        
        std::string serial = freenect2.getDefaultDeviceSerialNumber();
        
        if(mode == 1) //cpu
        {
            if(!pipeline){
                pipeline = new libfreenect2::CpuPacketPipeline();
                initialized_device = true;
            }else{
                 initialized_device = false;
                return -1;
            }
        }
        else if(mode == 2)
        {
            
            if(!pipeline){
                pipeline = new libfreenect2::OpenGLPacketPipeline();
                 initialized_device = true;
            }else{
                 initialized_device = false;
                return -1;
            }
            //std::cout << "OpenGL pipeline is not supported!" << std::endl;
            
        }
        else if(mode == 3)
        {
            
            if(!pipeline){
                pipeline = new libfreenect2::OpenCLPacketPipeline();
                 initialized_device = true;
            }else{
                initialized_device = false;
                return -1;
            }
            //std::cout << "OpenCL pipeline is not supported!" << std::endl;
        }else{
            initialized_device = false;
            return -1;
        }
        
        
        if(pipeline)
        {
            initialized_device = true;
            dev = freenect2.openDevice(serial, pipeline);
        }
        else
        {
            std::string serial = "0123456789";
            dev = freenect2.openDevice(serial);
        }
        
        if(initialized_device){
        
            //signal(SIGINT, &Device::sigint_handler);
            listener = new libfreenect2::SyncMultiFrameListener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
        

            dev->setColorFrameListener(listener);
            dev->setIrAndDepthFrameListener(listener);
            dev->start();
        
            std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
            std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;
            
            mSerialKinect = dev->getSerialNumber();
            
            registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
        }
        
        if(registration != NULL){
            initialized_device = true;
            return 1;
        }else{
            initialized_device = false;
            return -1;
        }
        
        return 1;
    }
    

    
    void Device::closeKinect()
    {
        std::cout<<"closing Kinect v2 :"<<mSerialKinect<<std::endl;
        if(initialized_device){
            dev->stop();
            dev->close();
        
            if( registration != NULL){
                std::cout<<"eror 4"<<std::endl;
                delete registration;
            }
        }
        
        if(depthData != NULL){
            delete depthData;
        }
        
        if(irData != NULL){
            delete  irData;
        }
        
        if(colorData != NULL){
            delete colorData;
        }
    }
    
    void Device::open(int mode)
    {
        int retVal = openKinect(mode);
        
        if(retVal == 1){
             mKinectThread = std::thread(&Device::updateKinect, this);
            initialized_device = true;
            std::cout<<"initilize depth thread"<<std::endl;
        }else{
            initialized_device = false;
        }
    }
    
    void Device::updateKinect()
    {
        libfreenect2::FrameMap frames;
        

        //MAIN THREAD
        while(initialized_device){
            listener->waitForNewFrame(frames);
            
            libfreenect2::Frame * rgb   = frames[libfreenect2::Frame::Color];
            libfreenect2::Frame * ir    = frames[libfreenect2::Frame::Ir];
            libfreenect2::Frame * depth = frames[libfreenect2::Frame::Depth];
            
            libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4);
            
            
            //COLOR DEPTH MAPPING
            registration->apply(rgb,depth,&undistorted,&registered);
            
            
            int pDepthTmp = 0;
            int pDepthEnd = (FRAME_SIZE_DEPTH);
            int indexDepth = 0;
            int indexIR = 0;
            
            //DEPTH AND IR
            while(pDepthTmp < pDepthEnd){
                
                //DEPTH
                int pixelDepthA = depth->data[indexDepth++];//noisy
                int pixelDepthR = depth->data[indexDepth++];//noisy lines
                int pixelDepthG = depth->data[indexDepth++]; //great but with white lines
                int pixelDepthB = depth->data[indexDepth++]; // gray depth no scale
                
                //IR
                int pixelIrB = ir->data[indexIR++];//noise
                int pixelIrG = ir->data[indexIR++];//noise
                int pixelIrR = ir->data[indexIR++]; //gray with noise
                int pixelIrA = ir->data[indexIR++]; // gray with no light
                
                //DEPTH
                depthData[pDepthTmp] =  colorByte2Int(pixelDepthG);

                //IR
                irData[pDepthTmp] =  colorByte2Int(pixelIrR);
              
            
                //gray strips just like the kinect v2 sdk
                // float gray = (float)pixelB * 0.2126 * (float)pixelG * 0.7152f + (float)pixelR * 0.0722;
                // depthData[pDepthTmp] = colorByte2Int((uint32_t)gray);
                //uint32_t gray =  ((float)pixelB * valB)  +  ((float)pixelG * valG)  +  ((float)pixelR * valR) + ((float)pixelA * valA);
                //float gray = pow(((float)pixelB * valB), gamma)  * pow(((float)pixelG * valG), gamma)  + pow(((float)pixelR * valR), gamma) + pow(((float)pixelA * valA), gamma);
                //int gray =  int(((pixelDepthB) * valB)  +  ((pixelDepthG) * valG)  +  ((pixelDepthR)* valR) + ((pixelDepthA) * valA));

                
                pDepthTmp++;

            }
        
          
    
            //Converts Color Format to RGB.
            int pColorEnd = (FRAME_SIZE_COLOR);
            int indexColor = 0;
            int pColorIndex = 0;
            
            while(pColorIndex < pColorEnd){
                int pixelColB =  rgb->data[indexColor++];
                int pixelColG =  rgb->data[indexColor++];
                int pixelColR =  rgb->data[indexColor++];
                int pixelColA =  rgb->data[indexColor++];
                
                //ABGR format
                
                colorData[pColorIndex] = colorByte2Int(pixelColR, pixelColG, pixelColB, pixelColA);
                pColorIndex++;
            }
            
            //framw listener
            listener->release(frames);
        }
        
    }
    
    float Device::clamp(float value, float min, float max) {
        return value < min ? min : value > max ? max : value;
    }
    
    float Device::lmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {
        
        if (fabs(inputMin - inputMax) < FLT_EPSILON){
            return outputMin;
        } else {
            float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
            
            if( clamp ){
                if(outputMax < outputMin){
                    if( outVal < outputMax )outVal = outputMax;
                    else if( outVal > outputMin )outVal = outputMin;
                }else{
                    if( outVal > outputMax )outVal = outputMax;
                    else if( outVal < outputMin )outVal = outputMin;
                }
            }
            return outVal;
        }
    }

    
    int Device::colorByte2Int(int gray, int alpha)
    {
        gray = gray & 0xffff;
        alpha = alpha & 0xffff;
        return (alpha << 24) | (gray << 16) | (gray << 8) | gray;
    }
    
    int Device::colorByte2Int(int gray)
    {
        gray = gray & 0xffff;
        return 0xff000000 | (gray << 16) | (gray << 8) | gray;
    }
    
    
    uint32_t Device::colorByte2Int(int r, int g, int b, int  a)
    {
        //return a | ( int(r) << 8 ) | ( int(g) << 16 ) | ( int(b) << 24 );
        //return (a << 24) | (r << 16) | (g << 8) | b;
       // gray = gray & 0xffff;
       // return 0xff000000 | (gray << 16) | (gray << 8) | gray;
        
        return a << 24 | (r << 16) | (g << 8) | b;
    }
    
    //depth
    uint32_t * Device::JNI_GetDepth()
    {
        return depthData;
    }
    
    uint32_t *  Device::JNI_GetColor()
    {
        return colorData;
    }
    
    uint32_t *  Device::JNI_GetIr()
    {
        return irData;
    }
    
    
    bool Device::isDepthReady()
    {
        return  (depthData != NULL) ? true : false;
    }
}