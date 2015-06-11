#include "RGBPacketProcessor.h"

namespace libfreenect2
{

RGBPacketProcessor::RGBPacketProcessor(){
    newFrame();
}

RGBPacketProcessor::~RGBPacketProcessor(){

}

void RGBPacketProcessor::newFrame(){
    frame = new Frame(1920, 1080, 3);
}

void RGBPacketProcessor::process(const libfreenect2::RgbPacket &packet){
    /*
    ofBuffer tmp;
    tmp.set( ( char * )packet.jpeg_buffer, (unsigned int)packet.jpeg_buffer_length);
    
    if( ofLoadImage(pix, tmp) ){
    
      memcpy(frame->data, pix.getPixels(), pix.size());
    
      if(listener_->addNewFrame(Frame::Color, frame)){
        newFrame();
      }
    }
    */
}

};