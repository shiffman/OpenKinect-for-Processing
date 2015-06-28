//
//  Base on the of Protonect.h of Theodore Watson.
//
//
#ifndef PROTONECT_H_
#define PROTONECT_H_

#include <iostream>
#include <signal.h>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/threading.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>



namespace nect{
class Protonect{

    public:
    
        Protonect();
    
        int openKinect(int mode);
    
        void closeKinect();
  
    protected:
        bool protonect_shutdown;
    
        libfreenect2::Freenect2  freenect2;
        libfreenect2::Freenect2Device *dev = 0;
        libfreenect2::PacketPipeline *pipeline = 0;
        libfreenect2::Registration* registration = 0;

};
}
#endif