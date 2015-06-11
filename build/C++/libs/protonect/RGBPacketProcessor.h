#pragma once

#include <libfreenect2/rgb_packet_processor.h>
#include <libfreenect2/frame_listener.h>

namespace libfreenect2{

class RGBPacketProcessor : public RgbPacketProcessor{

    public:
        RGBPacketProcessor();
        ~RGBPacketProcessor();
    
         void process(const libfreenect2::RgbPacket &packet);

    protected:
        void newFrame();
        Frame *frame;
        //ofPixels pix;

};

};