//
//  ofProtonect.h
//
//  Created by Theodore Watson on 6/27/14.
//
//

#include "ofMain.h"

#include <libfreenect2/tables.h>
#include <libfreenect2/usb/event_loop.h>
#include <libfreenect2/usb/transfer_pool.h>
#include <libfreenect2/rgb_packet_stream_parser.h>
#include <libfreenect2/rgb_packet_processor.h>
#include <libfreenect2/depth_packet_stream_parser.h>
#include <libfreenect2/frame_listener.h>
#include "ofRGBPacketProcessor.h"

#include "ofAppGLFWWindow.h"
#include "ofAppRunner.h"

class ofProtonect{

    public:
    
        ofProtonect();
    
        int openKinect(std::string dataPath);
        void updateKinect(ofPixels & rgbPixels, ofFloatPixels & depthPixels);
        int closeKinect();
    
        void exit(ofEventArgs & args);
  
    protected:
        bool bOpened;
    
        libfreenect2::FrameMap frames;
        libfreenect2::FrameListener * frame_listener;
        libfreenect2::usb::IsoTransferPool * depth_iso_transfersPtr = NULL;
        libfreenect2::usb::BulkTransferPool * rgb_bulk_transfersPtr = NULL;
        libusb_device_handle *handle;
        libfreenect2::usb::EventLoop usb_loop;
        libusb_device *dev;

        libfreenect2::ofRGBPacketProcessor  * rgb_processor;

        libfreenect2::usb::BulkTransferPool * rgb_bulk_transfers;
        libfreenect2::RgbPacketStreamParser * rgb_packet_stream_parser;

//        libfreenect2::usb::BulkTransferPool rgb_bulk_transfers(handle, 0x83);
//        libfreenect2::RgbPacketStreamParser rgb_packet_stream_parser(&rgb_processor);

        libfreenect2::CpuDepthPacketProcessor * depth_processor;
        libfreenect2::usb::IsoTransferPool  * depth_iso_transfers;

//        libfreenect2::usb::IsoTransferPool depth_iso_transfers(handle, 0x84);
        libfreenect2::DepthPacketStreamParser * depth_packet_stream_parser;

        bool should_resubmit = true;
        uint32_t num_iso_requests_outstanding = 0;

    const char* speed_name[5] =
      { "Unknown", "1.5 Mbit/s (USB LowSpeed)", "12 Mbit/s (USB FullSpeed)", "480 Mbit/s (USB HighSpeed)", "5000 Mbit/s (USB SuperSpeed)" };

};