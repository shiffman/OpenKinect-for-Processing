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
        depthData       = (uint32_t *)malloc(FRAME_SIZE_DEPTH * sizeof(uint32_t));
        irData          = (uint32_t *)malloc(FRAME_SIZE_DEPTH * sizeof(uint32_t));
        colorData       = (uint32_t *)malloc(FRAME_SIZE_COLOR * sizeof(uint32_t));
        undisortedData  = (uint32_t *)malloc(FRAME_SIZE_DEPTH * sizeof(uint32_t));
        registeredData  = (uint32_t *)malloc(FRAME_SIZE_DEPTH * sizeof(uint32_t));
        rawDepthData    = (uint32_t *)malloc(FRAME_SIZE_DEPTH * sizeof(uint32_t));
        
        //XYZ data, depth size times 3
        depthCameraData = (float *)malloc(FRAME_SIZE_DEPTH * 3 * sizeof(float));
        
        mSerialKinect = "";
        mNumDevices   = 0;
        
        //enables
        enableDepth       = false;
        enableVideo       = false;
        enableIR          = false;
        enableRegistered  = false;
        
        //toggles
        toggleDepth      = false;
        toggleVideo      = false;
        toggleIR         = false;
        toggleRegistered = false;
        
        enumerateDevices();
    }
    
    void Device::enumerateDevices()
    {
        libfreenect2::Freenect2  libFreenect2;
        
        mNumDevices = libFreenect2.enumerateDevices();
        if(mNumDevices == 0)
        {
             std::cerr << "No Device Connected!" << std::endl;
        }else{
            std::cout <<mNumDevices<<" Device Connected!" << std::endl;
        }
        
    }
    
    //open the kinect
    void Device::openKinect(int index)
    {
        if(mNumDevices == 0)
        {
            std::cerr << "Cannot Find Devices" << std::endl;
            initialized_device = false;
            return;
        }
        
        if(!pipeline){
            pipeline = new libfreenect2::OpenCLPacketPipeline();
             initialized_device = true;
        }else{
            std::cerr << "OpenCL pipeline is not supported!" << std::endl;
            initialized_device = false;
            return;
        }

        if(pipeline)
        {
           
            //open the kinect with a specific Serial number
            std::cout<<"Devce: "<<index<<std::endl;
            dev = freenect2.openDevice(index, pipeline);
            initialized_device = true;
            
            if(dev == 0){
                initialized_device = false;
                std::cerr << "no device connected or failure opening the default one!" << std::endl;
                return;
            }
        }
        else
        {
            std::cerr << "failed to start openCL packet pipeline" << std::endl;
            initialized_device = false;
            return;
        }
        
        if(initialized_device){
            
            //if enable registeres, video and depth  has to be activated
            if(enableRegistered){
                enableVideo = true;
                enableDepth = true;
                std::cout<<"Enable Kinect 2 Video + Depth Map"<<std::endl;
            }
            
            if(enableVideo){
                toggleVideo = true;
                std::cout<<"Enable Kinect 2 Video"<<std::endl;
            }
            if(enableIR){
                toggleIR = true;
                std::cout<<"Enable Kinect 2 IR"<<std::endl;
            }
            
            if(enableDepth){
                toggleDepth = true;
                std::cout<<"Enable Kinect 2 Depth"<<std::endl;
            }
            
            int flags = 0;
            flags |= enableVideo ? libfreenect2::Frame::Color : 0;
            flags |= enableIR ? libfreenect2::Frame::Ir : 0;
            flags |= enableDepth ? libfreenect2::Frame::Depth : 0;
        
            listener = new libfreenect2::SyncMultiFrameListener(flags);
        
            if(enableVideo){
                dev->setColorFrameListener(listener);
            }
            
            if(enableIR || enableDepth){
                dev->setIrAndDepthFrameListener(listener);
            }
            
            dev->start();
        
            std::cout << "Device Serial: " << dev->getSerialNumber() << std::endl;
            std::cout << "Device Firmware: " << dev->getFirmwareVersion() << std::endl;
            
            if(enableRegistered){
                registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
                toggleRegistered = true;
                
                if(registration == NULL){
                    std::cerr<<"error opening registered mapping process"<<std::endl;
                    return;
                }
            }
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
        
        if(depthCameraData != NULL){
            delete depthCameraData;
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
        
        libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4);
                                                                 
        //MAIN THREAD
        while(initialized_device){
            listener->waitForNewFrame(frames);
            
            if(enableRegistered){
                
                libfreenect2::Frame *  rgb   = frames[libfreenect2::Frame::Color];
                memcpy(colorData, reinterpret_cast<const uint32_t *>(rgb->data), 1920 * 1080 * 4);
                
                libfreenect2::Frame *  depth = frames[libfreenect2::Frame::Depth];
                memcpy(newDepth, reinterpret_cast<const float * >(depth->data), FRAME_BYTE_SIZE_DEPTH);
                
                 //Mappers RGB + Depth
                registration->apply(rgb, depth, &undistorted, &registered);
                memcpy(newUndisorted, reinterpret_cast<const float * >(undistorted.data), FRAME_BYTE_SIZE_DEPTH);
                memcpy(registeredData, reinterpret_cast<const uint32_t * >(registered.data), FRAME_BYTE_SIZE_DEPTH);
            }else if(enableVideo && !enableDepth){
                
                libfreenect2::Frame *  rgb   = frames[libfreenect2::Frame::Color];
                memcpy(colorData, reinterpret_cast<const uint32_t *>(rgb->data), 1920 * 1080 * 4);
            }else if( !enableVideo && enableDepth ){
                
                libfreenect2::Frame *  depth = frames[libfreenect2::Frame::Depth];
                memcpy(newDepth, reinterpret_cast<const float * >(depth->data), FRAME_BYTE_SIZE_DEPTH);
            }else if(enableVideo && enableDepth && !enableRegistered){
                
                libfreenect2::Frame *  rgb   = frames[libfreenect2::Frame::Color];
                memcpy(colorData, reinterpret_cast<const uint32_t *>(rgb->data), 1920 * 1080 * 4);
                
                libfreenect2::Frame *  depth = frames[libfreenect2::Frame::Depth];
                memcpy(newDepth, reinterpret_cast<const float * >(depth->data), FRAME_BYTE_SIZE_DEPTH);
            }
        

            if(enableIR){
                libfreenect2::Frame *  ir    = frames[libfreenect2::Frame::Ir];
                memcpy(newIr, reinterpret_cast<const float * >(ir->data), FRAME_BYTE_SIZE_DEPTH);
            }
       
                int indexFD = 0;
                int pIndexEnd = (FRAME_SIZE_DEPTH);
                
                int indexX = 0;
                int indexY = 0;
                int cameraXYZ = 0;
                while(indexFD < pIndexEnd){
                    float depth = newDepth[indexFD];
                    
                    //Depth
                    //0.0566666f -> (value/45000)* 255
                    rawDepthData[indexFD] = uint32_t(depth);
                   
                    //IR
                    irData[indexFD]  = colorByte2Int((uint32_t(newIr[indexFD]*0.0566666f)>>2));
      
                    //undisorted
                    undisortedData[indexFD]  = colorByte2Int(uint32_t(newUndisorted[indexFD]*0.0566666f));
                    
                    
                    depthData[indexFD]  = colorByte2Int(uint32_t(depth*0.0566666f));
                    
                    //evaluates the depth XYZ position;
                   
                    depthCameraData[cameraXYZ++] = (indexX - dev->getIrCameraParams().cx) * depth / dev->getIrCameraParams().fx;//x
                    depthCameraData[cameraXYZ++] = (indexY - dev->getIrCameraParams().cy) * depth / dev->getIrCameraParams().fy; //y
                    depthCameraData[cameraXYZ++] = depth; //z
                    
                    indexX++;
                    if(indexX >= 512){ indexX=0; indexY++;}

                    indexFD++;
              //  }
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
    
    //------JNI functions
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
    
    float * Device::JNI_GetDepthCameraPositions()
    {
        return depthCameraData;
    }
    
           
    //----HELP functions-----
    //return float pointer with x, y, and z values
    float  *   Device::depthToCameraSpace(int & x, int & y, float & depth)
    {
        float * point  = new float[3];
        point[2] = (depth); // z, convert from mm to meters
        point[0] = (x - dev->getIrCameraParams().cx) * point[2] / dev->getIrCameraParams().fx; //x
        point[1] = (y - dev->getIrCameraParams().cy) * point[2] / dev->getIrCameraParams().fy; //y
        return point;
    }
    
    float Device::clamp(float value, float min, float max)
    {
        return value < min ? min : value > max ? max : value;
    }
    
    float Device::lmap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp)
    {
        
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