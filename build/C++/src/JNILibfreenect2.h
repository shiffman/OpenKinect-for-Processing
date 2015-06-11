/*
 *  JNILibfreenect2.h
 *  JNILibfreenect2
 *
 *  Created by tom on 6/7/15.
 *  Copyright (c) 2015 tom. All rights reserved.
 *
 */

#pragma once

#include <iostream>
#include "Protonect.h"

namespace  LibFreenect2{
    
    class Device
    {
    public:
        Device();
        void open();
        void stop();
        void setValue(int val){v = val;}
        void HelloWorld(const char *);
    private:
        int v;
         Protonect  * protonect;
    };

}

