//
//  JNILibfreenect2.cpp
//  JNILibfreenect2
//
//  Created by tom on 6/10/15.
//  Copyright (c) 2015 tom. All rights reserved.
//

#include <stdio.h>

#include "JNILibfreenect2.h"

namespace LibFreenect2 {
    Device::Device()
    {
        std::cout<<"init kinect 2"<<std::endl;
    }
    
    void Device::stop()
    {
        std::cout<<"stopging kinect v2"<<std::endl;
        
    }
}