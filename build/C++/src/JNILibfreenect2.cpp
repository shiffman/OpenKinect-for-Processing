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
    Device::Device()
    {
        std::cout<<" Hello protonect "<<v<<std::endl;
        
        protonect = new nect::Protonect();
        
    }
    void Device::open()
    {
        int retVal = protonect->openKinect(1);
    }
    
    void Device::stop()
    {
       // protonect->closeKinect();
        std::cout<<"stopging kinect v2 "<<v<<std::endl;
        
    }
}