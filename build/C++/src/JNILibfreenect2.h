/*
 *  JNILibfreenect2.h
 *  JNILibfreenect2
 *
 *  Created by tom on 6/7/15.
 *  Copyright (c) 2015 tom. All rights reserved.
 *
 */

#ifndef JNILibfreenect2_
#define JNILibfreenect2_





/* The classes below are exported */
#pragma GCC visibility push(default)

#include <iostream>

namespace  LibFreenect2{
    
    class Device
    {
    public:
        Device();
        void stop();
        void HelloWorld(const char *);
    private:
    };

}
#pragma GCC visibility pop
#endif
