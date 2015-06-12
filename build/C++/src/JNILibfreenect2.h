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
#include "Protonect.h"

/* The classes below are exported */
#pragma GCC visibility push(default)

namespace  openKinect2{
    
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
        nect::Protonect  * protonect;
    };

}
#pragma GCC visibility pop
#endif


