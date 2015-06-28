/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2011 individual OpenKinect contributors. See the CONTRIB file
 * for details.
 *
 * This code is licensed to you under the terms of the Apache License, version
 * 2.0, or, at your option, the terms of the GNU General Public License,
 * version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
 * or the following URLs:
 * http://www.apache.org/licenses/LICENSE-2.0
 * http://www.gnu.org/licenses/gpl-2.0.txt
 *
 * If you redistribute this file in source form, modified or unmodified, you
 * may:
 *   1) Leave this header intact and distribute it under the same terms,
 *      accompanying it with the APACHE20 and GPL20 files, or
 *   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
 *   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
 * In all cases you must keep the copyright notice intact and include a copy
 * of the CONTRIB file.
 *
 * Binary distributions must follow the binary distribution requirements of
 * either License.
 */

#include "Protonect.h"


#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>


namespace  nect {
    
    Protonect::Protonect(){
        
    }
    
    int Protonect::openKinect(int mode)
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
        
        protonect_shutdown = false;
       
        
        libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
        
        libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4);
        
        dev->setColorFrameListener(&listener);
        dev->setIrAndDepthFrameListener(&listener);
        dev->start();
        
        std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
        std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;
        
        registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
        
        if(registration != NULL)
            return 1;
        else
            return -1;
        
        return 1;
    }
    
    void Protonect::closeKinect()
    {
        dev->stop();
        dev->close();
        
        if( registration != NULL)
            delete registration;
    }
    

}
