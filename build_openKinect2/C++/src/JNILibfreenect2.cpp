//
//  JNILibfreenect2.cpp
//  JNILibfreenect2
//
//  Created by tom on 6/10/15.
//  Copyright (c) 2015 Thomas Sanchez Lengeling. All rights reserved.
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
        initialized_device = false;
        
        //load memory
        depthData      = (uint32_t *)malloc(FRAME_SIZE_DEPTH * sizeof(uint32_t));
        irData         = (uint32_t *)malloc(FRAME_SIZE_DEPTH * sizeof(uint32_t));
        colorData      = (uint32_t *)malloc(FRAME_SIZE_COLOR * sizeof(uint32_t));
        undisortedData = (uint32_t *)malloc(FRAME_SIZE_DEPTH * sizeof(uint32_t));
        registeredData = (uint32_t *)malloc(FRAME_SIZE_DEPTH * sizeof(uint32_t));
        rawDepthData   = (uint32_t *)malloc(FRAME_SIZE_DEPTH * sizeof(uint32_t));
        
        mSerialKinect = "";
        mNumDevices   = 0;
        
        enumerateDevices();
    }
    
    void Device::enumerateDevices()
    {
        libfreenect2::Freenect2  libFreenect2;
        
        mNumDevices = libFreenect2.enumerateDevices();
        if(mNumDevices == 0)
        {
             std::cout << "No Device Connected!" << std::endl;
        }else{
            std::cout <<mNumDevices<<" Device Connected!" << std::endl;
        }
        
    }
    
    //open the kinect
    void Device::openKinect(int index)
    {
        if(mNumDevices == 0)
        {
            initialized_device = false;
            return;
        }
        
        if(!pipeline){
            pipeline = new libfreenect2::OpenCLPacketPipeline();
             initialized_device = true;
        }else{
            std::cout << "OpenCL pipeline is not supported!" << std::endl;
            initialized_device = false;
            return;
        }

        if(pipeline)
        {
            initialized_device = true;
            
            //open the kinect with a specific Serial number
            std::cout<<"Devce :"<<index<<std::endl;
            dev = freenect2.openDevice(index, pipeline);
        }
        else
        {
            initialized_device = false;
           // std::string serial = "0123456789";
           // dev = freenect2.openDevice(serial);
            return;
        }
        
        if(initialized_device){
        
            //signal(SIGINT, &Device::sigint_handler);
            listener = new libfreenect2::SyncMultiFrameListener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
        

            dev->setColorFrameListener(listener);
            dev->setIrAndDepthFrameListener(listener);
            dev->start();
        
            std::cout << "Device Serial: " << dev->getSerialNumber() << std::endl;
            std::cout << "Device Firmware: " << dev->getFirmwareVersion() << std::endl;
            
            registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
        }
        
        if(registration != NULL){
            initialized_device = true;
        }else{
            initialized_device = false;
            return;
        }
        
        if(initialized_device){
            mKinectThread = std::thread(&Device::updateKinect, this);
        }else{
            return;
        }

    }
    
    //close the kinect
    void Device::closeKinect()
    {
        std::cout<<"closing Kinect v2 :"<<mSerialKinect<<std::endl;
        if(initialized_device){
            dev->stop();
            dev->close();
        
            if( registration != NULL){
                delete registration;
            }
        }
        
        if(depthData != NULL){
            delete depthData;
        }
        
        if(rawDepthData != NULL){
            delete rawDepthData;
        }
        
        if(irData != NULL){
            delete  irData;
        }
        
        if(colorData != NULL){
            delete colorData;
        }
        
        if(undisortedData != NULL){
            delete undisortedData;
        }
        
        if(registeredData != NULL){
            delete registeredData;
        }
    }
    
    //get number of devices
    int Device::getDeviceCount()
    {
        return mNumDevices;
    }
    
    //get the serial number from the kinect
    std::string Device::getSerial(int index )
    {
        if(mNumDevices  > 0){
            mSerialKinect = freenect2.getDeviceSerialNumber(index);
        }
        return mSerialKinect;
    }
    
    //update the kinect
    void Device::updateKinect()
    {
        libfreenect2::FrameMap frames;
        

        //Temporary arrays
        float * newDepth = new float[FRAME_SIZE_DEPTH];
        float * newIr    = new float[FRAME_SIZE_DEPTH];
        float * newUndisorted =  new float[FRAME_SIZE_DEPTH];
        
        
        //MAIN THREAD
        while(initialized_device){
            listener->waitForNewFrame(frames);
            
            libfreenect2::Frame * rgb   = frames[libfreenect2::Frame::Color];
            libfreenect2::Frame * ir    = frames[libfreenect2::Frame::Ir];
            libfreenect2::Frame * depth = frames[libfreenect2::Frame::Depth];
            
            libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4);
            
            //COLOR DEPTH MAPPING
            registration->apply(rgb, depth, &undistorted, &registered);
            
       
            //Memory copies to the data arrays to send them to processing via JNI
            //DEPTH
            memcpy(newDepth, reinterpret_cast<const float * >(depth->data), FRAME_BYTE_SIZE_DEPTH);
            
            //copy raw depth
            memcpy(rawDepthData, reinterpret_cast<const float * >(depth->data), FRAME_BYTE_SIZE_DEPTH);
            
            //IR
            memcpy(newIr, reinterpret_cast<const float * >(ir->data), FRAME_BYTE_SIZE_DEPTH);
            
            //Color
            memcpy(colorData, reinterpret_cast<const uint32_t *>(rgb->data), 1920 * 1080 * 4);
            
            //Mapers
            //undisorted
            memcpy(newUndisorted, reinterpret_cast<const float * >(undistorted.data), FRAME_BYTE_SIZE_DEPTH);
            
            //registered
            memcpy(registeredData, reinterpret_cast<const uint32_t * >(registered.data), FRAME_BYTE_SIZE_DEPTH);
            
        
            //convert depth, ir to processing pixels
            int indexFD = 0;
            int pIndexEnd = (FRAME_SIZE_DEPTH);
            
            while(indexFD < pIndexEnd){
                
                //0.0566666f -> (value/45000)* 255
                
                //Depth
                depthData[indexFD]  = colorByte2Int(uint32_t(newDepth[indexFD]*0.0566666f));
                
    
                //IR
                irData[indexFD]  = colorByte2Int((uint32_t(newIr[indexFD]*0.0566666f)>>4));
                
                //undisorted
                undisortedData[indexFD]  = colorByte2Int(uint32_t(newUndisorted[indexFD]*0.0566666f));
                
                indexFD++;
            }
            
            
            //framw listener
            listener->release(frames);
        }
        
        //clean up
        if(newDepth != NULL) delete newDepth;
        if(newIr != NULL) delete newIr;
        if(newUndisorted != NULL) delete newUndisorted;
        
    }
    
    bool Device::isKinectReady()
    {
        return  initialized_device;
    }
    
    //depth
    uint32_t * Device::JNI_GetDepth()
    {
        return depthData;
    }
    
    //Raw Depth
    uint32_t * Device::JNI_GetRawDepth()
    {
        return rawDepthData;
    }
    
    uint32_t *  Device::JNI_GetColor()
    {
        return colorData;
    }
    
    uint32_t *  Device::JNI_GetIr()
    {
        return irData;
    }
    
    uint32_t *  Device::JNI_GetUndistorted()
    {
        return undisortedData;
    }
    
    uint32_t *  Device::JNI_GetRegistered()
    {
        return registeredData;
    }
    
    //----HELP functions-----
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
    
    uint32_t Device::colorByte2Int(uint32_t gray)
    {
        gray = gray & 0xffff;
        return 0xff000000 | (gray << 16) | (gray << 8) | gray;
    }
    
    
    uint32_t Device::colorByte2Int(uint8_t r, uint8_t g, uint8_t b)
    {
         return 0xff000000  | (r << 16) | (g << 8) | b;
    }
    
    uint32_t Device::colorByte2Int(uint8_t r, uint8_t g, uint8_t b, uint8_t  a)
    {
        return a << 24 | (r << 16) | (g << 8) | b;
    }
    

}