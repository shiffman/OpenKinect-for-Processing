//
//  Created by Yuya Hanai on 10/16/14.
//

#include "ofProtonect2.h"

#include <libfreenect2/async_packet_processor.h>
#include <libfreenect2/depth_packet_processor.h>




using namespace libfreenect2;

class PassThroughRgbPacketProcessor : public RgbPacketProcessor
{
public:
    PassThroughRgbPacketProcessor() : rgb_packet_frame(NULL) {}
    virtual ~PassThroughRgbPacketProcessor() {}
protected:
    Frame *rgb_packet_frame;
    
    virtual void process(const libfreenect2::RgbPacket &packet)
    {
        rgb_packet_frame = new Frame(packet.jpeg_buffer_length, 1, 1);
        std::copy(packet.jpeg_buffer, packet.jpeg_buffer + packet.jpeg_buffer_length, rgb_packet_frame->data);
        
        if(listener_->onNewFrame(Frame::Color, rgb_packet_frame))
        {
        }
        else
        {
            delete rgb_packet_frame;
        }
        rgb_packet_frame = NULL;

    }
};

//----------------------------------------------------------------

class PassThroughDepthPacketProcessor : public DepthPacketProcessor
{
public:
    PassThroughDepthPacketProcessor() :
    p0tableInvalidated(false),
    ir_packet_frame(NULL)
    {
        for(int i = 0; i < 3; ++i) {
       //     p0tablepx[i].allocate(512, 424, 1);
        //    p0tablepx[i].set(0);
        }
    }
    virtual ~PassThroughDepthPacketProcessor() {}
    
   // ofShortPixels p0tablepx[3];
    bool p0tableInvalidated;
protected:
    Frame *ir_packet_frame;
    
    virtual void process(const libfreenect2::DepthPacket &packet)
    {
        ir_packet_frame = new Frame(packet.buffer_length, 1, 1);
        std::copy(packet.buffer, packet.buffer + packet.buffer_length, ir_packet_frame->data);
        
        if(listener_->onNewFrame(Frame::Ir, ir_packet_frame))
        {
        }
        else
        {
            delete ir_packet_frame;
        }
        ir_packet_frame = NULL;
    }
    
    virtual void loadP0TablesFromCommandResponse(unsigned char* buffer, size_t buffer_length)
    {
        // TODO: check known header fields (headersize, tablesize)
        libfreenect2::protocol::P0TablesResponse* p0table = (libfreenect2::protocol::P0TablesResponse*)buffer;
        
        if(buffer_length < sizeof(libfreenect2::protocol::P0TablesResponse))
        {
            std::cerr << "[PassThroughDepthPacketProcessor::loadP0TablesFromCommandResponse] P0Table response too short!" << std::endl;
            return;
        }
        
        size_t n = 512 * 424;
      //  std::copy(p0table->p0table0, p0table->p0table0 + n, p0tablepx[0].getPixels());
     //   std::copy(p0table->p0table1, p0table->p0table1 + n, p0tablepx[1].getPixels());
     //   std::copy(p0table->p0table2, p0table->p0table2 + n, p0tablepx[2].getPixels());
        
        for(int i = 0; i < 3; ++i) {
      //      p0tablepx[i].mirror(true, false);
        }
        
        p0tableInvalidated = true;
        std::cerr << "[PassThroughDepthPacketProcessor::loadP0TablesFromCommandResponse] received." << std::endl;
    }
};

//----------------------------------------------------------------


class ofPacketPipeline : public PacketPipeline
{
private:
    RgbPacketStreamParser *rgb_parser_;
    DepthPacketStreamParser *depth_parser_;
    
    RgbPacketProcessor *rgb_processor_;
    BaseRgbPacketProcessor *async_rgb_processor_;
    DepthPacketProcessor *depth_processor_;
    BaseDepthPacketProcessor *async_depth_processor_;
public:
    ofPacketPipeline(int cldeviceindex);
    virtual ~ofPacketPipeline();
    
    virtual PacketParser *getRgbPacketParser() const;
    virtual PacketParser *getIrPacketParser() const;
    
    virtual RgbPacketProcessor *getRgbPacketProcessor() const;
    virtual DepthPacketProcessor *getDepthPacketProcessor() const;
};

//----------------------------------------------------------------

ofPacketPipeline::ofPacketPipeline(int cldeviceindex)
{
    rgb_parser_ = new RgbPacketStreamParser();
    depth_parser_ = new DepthPacketStreamParser();
    
    rgb_processor_ = new PassThroughRgbPacketProcessor();

   // string binpath = ofToDataPath("");
    OpenCLDepthPacketProcessor* depth_processor = new OpenCLDepthPacketProcessor(cldeviceindex);
    //OpenGLDepthPacketProcessor* depth_processor = new OpenGLDepthPacketProcessor(cldeviceindex);
    //CpuDepthPacketProcessor* depth_processor = new CpuDepthPacketProcessor();
    depth_processor->load11To16LutFromFile("11to16.bin");
    depth_processor->loadXTableFromFile("xTable.bin");
    depth_processor->loadZTableFromFile("zTable.bin");
    
    libfreenect2::DepthPacketProcessor::Config config;
    config.MinDepth = 0.4f;
    config.MaxDepth = 10.0f;
    config.EnableBilateralFilter = true;
    config.EnableEdgeAwareFilter = false;
    depth_processor->setConfiguration(config);
    
    depth_processor_ = depth_processor;
    
    async_rgb_processor_ = new AsyncPacketProcessor<RgbPacket>(rgb_processor_);
    async_depth_processor_ = new AsyncPacketProcessor<DepthPacket>(depth_processor_);
    
    rgb_parser_->setPacketProcessor(async_rgb_processor_);
    depth_parser_->setPacketProcessor(async_depth_processor_);
}

ofPacketPipeline::~ofPacketPipeline()
{
    delete async_rgb_processor_;
    delete async_depth_processor_;
    delete rgb_processor_;
    delete depth_processor_;
    delete rgb_parser_;
    delete depth_parser_;
}

ofPacketPipeline::PacketParser *ofPacketPipeline::getRgbPacketParser() const
{
    return rgb_parser_;
}

ofPacketPipeline::PacketParser *ofPacketPipeline::getIrPacketParser() const
{
    return depth_parser_;
}

RgbPacketProcessor *ofPacketPipeline::getRgbPacketProcessor() const
{
    return rgb_processor_;
}

DepthPacketProcessor *ofPacketPipeline::getDepthPacketProcessor() const
{
    return depth_processor_;
}

//----------------------------------------------------------------
bool ofProtonect2::open(int deviceIndex, int mode, int clindex)
{
    if (bOpen) {
        return false;
    }
    
    pipeline = new ofPacketPipeline(clindex);
    dev = freenect2.openDevice(deviceIndex, pipeline);
    
    if (dev == 0)
    {
        std::cout << "no device connected or failure opening the default one!" << std::endl;
        return false;
    }
    
    listener = new libfreenect2::SyncMultiFrameListener(mode);
    
    dev->setColorFrameListener(listener);
    dev->setIrAndDepthFrameListener(listener);

    bOpen = true;
    return true;
}


void ofProtonect2::start() {
    if (!bOpen) {
        return;
    }
    dev->start();
    
    std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
    std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;
}

void ofProtonect2::update() {
    if (!bOpen) {return;}
    listener->waitForNewFrame(frames);
    if (!bOpen) {return;}
    
    {
        libfreenect2::Frame *rgbFrame = frames[libfreenect2::Frame::Color];
        if (rgbFrame) {
            int sz = rgbFrame->width * rgbFrame->height * rgbFrame->bytes_per_pixel;
            jpeg.resize(sz);
            std::copy(rgbFrame->data, rgbFrame->data + sz, &jpeg.front());
        }
    }
    {
        libfreenect2::Frame *irFrame = frames[libfreenect2::Frame::Ir];
        if (irFrame) {
            int sz = irFrame->width * irFrame->height * irFrame->bytes_per_pixel;
            rawir.resize(sz);
            std::copy(irFrame->data, irFrame->data + sz, &rawir.front());
        }
    }
    {
        libfreenect2::Frame *depthFrame = frames[libfreenect2::Frame::Depth];
        if (depthFrame) {
            int sz = depthFrame->width * depthFrame->height * depthFrame->bytes_per_pixel;
            rawdepth.resize(sz);
            std::copy(depthFrame->data, depthFrame->data + sz, &rawdepth.front());
        }
    }
    
    listener->release(frames);
}

void ofProtonect2::close() {
    if (bOpen) {
        // TODO: restarting ir stream doesn't work!
        // TODO: bad things will happen, if frame listeners are freed before dev->stop() :(
        dev->stop();
        dev->close();
        pipeline = NULL;
    }
    bOpen = false;
}

