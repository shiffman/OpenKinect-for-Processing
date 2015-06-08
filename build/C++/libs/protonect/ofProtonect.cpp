//
//  ofProtonect.cpp
//
//  Created by Theodore Watson on 6/27/14.
//
//

#include "ofProtonect.h"


/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2011 individual OpenKinect contributors. See the CONTRIB file
 * for details.
 *
 * This code is licensed to you under the terms of the Apache License, version
 * 2.0, or, at your option, the terms of the GNU General Public License,
 * version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
 * or the following URLs:
 * http://www.apache.org/licenses/LICENSE-2.0
 * http://www.gnu.org/licenses/gpl-2.0.txt
 *
 * If you redistribute this file in source form, modified or unmodified, you
 * may:
 *   1) Leave this header intact and distribute it under the same terms,
 *      accompanying it with the APACHE20 and GPL20 files, or
 *   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
 *   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
 * In all cases you must keep the copyright notice intact and include a copy
 * of the CONTRIB file.
 *
 * Binary distributions must follow the binary distribution requirements of
 * either License.
 */


#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include <opencv2/opencv.hpp>

static uint32_t cmd_seq = 0;

static int perr(char const *format, ...)
{
	va_list args;
	int r;

	va_start(args, format);
	r = vfprintf(stderr, format, args);
	va_end(args);
	
	return r;
}

#define ERR_EXIT(errcode) do { perr("   %s\n", libusb_strerror((enum libusb_error)errcode)); return -1; } while (0)
#define CALL_CHECK(fcall) do { r=fcall; if (r < 0) ERR_EXIT(r); } while (0);

#define U1_ENABLE 0x30
#define U2_ENABLE 0x31

#define K_SUCCESS 0
#define K_ERROR -1
#define K_RESPONSE_COMPLETE 1
#define K_RESPONSE_DATA 2

enum KSensorStatus 
{
	KSENSOR_ENABLE = 0x0, 
	KSENSOR_DISABLE = 0x300 
};

enum KStreamStatus
{
	KSTREAM_DISABLE = 0x0,
	KSTREAM_ENABLE = 0x1
};

enum KModeStatus
{
	KMODE_DISABLE = 0x0,
	KMODE_ENABLE = 0x1
};

//Same magic from Kinect v1 audio protocol, representing June 2, 2009, the day after Project Natal announcement at E3 2009
#define KCMD_MAGIC 0x06022009

//Magic header for response complete packet
#define KCMD_RESPONSE_COMPLETE_MAGIC 0x0A6FE000
#define KCMD_RESPONSE_COMPLETE_LEN 16

#define KCMD_READ_DATA1 0x02
#define KCMD_INIT_STREAMS 0x09
#define KCMD_READ_VERSIONS 0x14
#define KCMD_READ_STATUS 0x16
#define KCMD_READ_DATA_PAGE 0x22
#define KCMD_READ_COUNT 0x26
#define KCMD_SET_STREAMING 0x2B
#define KCMD_SET_MODE 0x4B

typedef struct
{
	uint32_t magic;		//0x06022009
	uint32_t sequence;	//incremented sequence number, echoed in response complete packet
	uint32_t responseDataLen;	//requested max length of response (sometimes response is smaller)
	uint32_t command;	//function this command represents
	uint32_t reserved0;	//always zero (so far)
	uint32_t parameter;	//optional parameter, varies by command
	uint32_t reserved1; //always zero (so far)
	uint32_t reserved2; //always zero (so far)
	uint32_t reserved3; //always zero (so far)
} cmd_header;


int KSetSensorStatus(libusb_device_handle *handle, KSensorStatus KSensorStatus)
{
  uint16_t wFeature = 0;
  uint16_t wIndex = KSensorStatus;
  unsigned char* data = NULL;
  uint16_t wLength = 0;
  unsigned int timeout = 1000;

  printf("Setting sensor status: %s\n", (KSensorStatus == KSENSOR_ENABLE ? "Enable" : "Disable"));

  int r = libusb_control_transfer(handle, LIBUSB_RECIPIENT_INTERFACE, LIBUSB_REQUEST_SET_FEATURE, wFeature, wIndex, data, wLength, timeout);

  if (r < 0)
  {
    perr("Set sensor status error: %d\n", r);
    return K_ERROR;
  }

  return K_SUCCESS;
}

int KSetStreamingInterfaceStatus(libusb_device_handle *handle, KStreamStatus KStreamStatus)
{
  int altSetting = KStreamStatus;
  int interfaceNum = 1;

  printf("Setting stream status: %s\n", (KStreamStatus == KSTREAM_ENABLE ? "Enable" : "Disable"));

  int r = libusb_set_interface_alt_setting(handle, interfaceNum, altSetting);

  if (r < 0)
  {
    perr("Set stream status error: %d\n", r);
    return K_ERROR;
  }

  return K_SUCCESS;
}

cmd_header KInitCommand()
{
  cmd_header cmd;
  memset(&cmd, 0, sizeof(cmd_header));

  cmd.magic = KCMD_MAGIC;
  cmd.sequence = cmd_seq;
  cmd_seq++;

  return cmd;
}

int KSendCommand(libusb_device_handle *handle, cmd_header& cmd, int length)
{
  uint8_t endpoint = 0x2;
  int transferred = 0;
  int timeout = 1000;

  uint8_t* p_data = (uint8_t*) (&cmd);

  printf("Cmd seq %u func %#04x (%#x)\n", cmd.sequence, cmd.command, cmd.parameter);
  int r = libusb_bulk_transfer(handle, endpoint, p_data, length, &transferred, timeout);

  if (r != LIBUSB_SUCCESS)
  {
    perr("Cmd error: %d\n", r);
    return K_ERROR;
  }

  printf("Cmd sent, %u bytes sent\n", transferred);

  return K_SUCCESS;
}

int KReadCommandResponse(libusb_device_handle *handle, cmd_header& cmd, uint32_t dataLen, uint8_t* data, int* transferred)
{
  if (NULL == transferred)
  {
    perr("Cannot read command response with transferred == NULL");
    return K_ERROR;
  }

  uint8_t endpoint = 0x81;
  int timeout = 1000;

  printf("Cmd response seq %u func %#04x (%#x)\n", cmd.sequence, cmd.command, cmd.parameter);
  int r = libusb_bulk_transfer(handle, endpoint, data, dataLen, transferred, timeout);

  uint32_t completedBytes = *transferred;

  if (r != LIBUSB_SUCCESS)
  {
    perr("Cmd response error: %d\n", r);
    return K_ERROR;
  }

  printf("Cmd response success, %u bytes received\n", completedBytes);

  if (completedBytes > dataLen)
  {
    perr("Warning, buffer length (%u) smaller than transferred bytes (%u).", dataLen, completedBytes);
  }

  if (completedBytes == KCMD_RESPONSE_COMPLETE_LEN)
  {
    uint32_t* i_data = (uint32_t*) data;

    uint32_t tag = i_data[0];
    uint32_t seq = i_data[1];

    if (tag == KCMD_RESPONSE_COMPLETE_MAGIC)
    {
      printf("Cmd response completed\n");
      if (seq != cmd.sequence)
      {
        perr("Cmd response completed with wrong sequence number. Expected %u, Got %u\n", cmd.sequence, seq);
      }
      return K_RESPONSE_COMPLETE;
    }
  }
  return K_RESPONSE_DATA;
}

int KSendCommandReadResponse(libusb_device_handle *handle, cmd_header& cmd, int length, uint8_t* data, int* transferred)
{
  if (NULL == transferred)
  {
    perr("Cannot read command response with transferred == NULL");
    return K_ERROR;
  }

  printf("\n===\n\n");
  int r;

  r = KSendCommand(handle, cmd, length);

  if (r == K_ERROR)
  {
    return K_ERROR;
  }

  if (data != NULL && cmd.responseDataLen > 0)
  {
    //Read data response
    r = KReadCommandResponse(handle, cmd, cmd.responseDataLen, data, transferred);

    uint32_t completedBytes = *transferred;

    if (r == K_RESPONSE_DATA)
    {
      printf("Received cmd data %#04x (%#x), length: %u\n", cmd.command, cmd.parameter, completedBytes);
    }
    else if (r == K_RESPONSE_COMPLETE)
    {
      perr("Premature response complete for %#04x cmd (%#x)\n", cmd.command, cmd.parameter);
      return K_ERROR;
    }
    else
    {
      perr("Error in response for %#04x cmd (%#x)\n", cmd.command, cmd.parameter);
      return K_ERROR;
    }
  }

  //Read expected response complete
  uint8_t responseData[512];
  int response_xf = 0;
  r = KReadCommandResponse(handle, cmd, 512, responseData, &response_xf);

  if (r == K_RESPONSE_COMPLETE)
  {
    printf("Response complete for cmd %#04x (%#x)\n", cmd.command, cmd.parameter);
  }
  else
  {
    perr("Missing expected response complete for cmd %#04x (%#x)\n", cmd.command, cmd.parameter);
    return K_ERROR;
  }

  return K_SUCCESS;
}

int KGenericCommand(libusb_device_handle *handle, int command, int parameter, int length, int response = 0, uint8_t** buffer = NULL)
{
  int transferred = 0;
  uint8_t* data = NULL;

  cmd_header cmd = KInitCommand();
  cmd.responseDataLen = response;
  cmd.command = command;
  cmd.parameter = parameter;

  if ((response > 0) && (buffer != NULL))
  {
    data = new uint8_t[response];
    *buffer = data;
  }

  int r = KSendCommandReadResponse(handle, cmd, length, data, &transferred);

  if (r == K_ERROR)
  {
    printf("Error in cmd protocol %#04x (%#x)\n", cmd.command, cmd.parameter);
    delete[] data;
    return K_ERROR;
  }

  return transferred;
}

void hexdump( uint8_t* buffer, int size, const char* info ) {
  printf("dumping %d bytes of raw data from command %s: \n",size,info);
  int lines = size >> 4;
  if (size % 16 != 0) lines += 1;
  for (int i = 0; i < lines; i++)
  {
    printf("0x%04x:  ", i*16);
    for (int j = 0; j < 16; j++)
    {
      if (j < size) printf("%02x ",buffer[i*16+j]);
      else printf("   ");
    }
    printf("    ");
    for (int j = 0; (j < 16) && (j < size); j++)
    {
      char c = buffer[i*16+j];
      printf("%c",(((c<32)||(c>128))?'.':c));
    }
    printf("\n");
    size -= 16;
  }
}

int KReadData02(libusb_device_handle *handle)
{
  uint8_t* data = NULL;
  int res = KGenericCommand(handle, KCMD_READ_DATA1, 0x00, 20, 0x200, &data);

  if (data != NULL)
  {

    /**
     * somehow related to firmware version?
     *
     * 7 blocks
     *
     * 01 00 uint16 = 1
     * 5A 0B uint16 = 2906
     * F6 0C uint16 = 3318
     *
     * my firmware version reported by SDK 1 1 3318 0 7 - coincidence?
     *
     * 01 00 01 00 5A 0B 00 00 00 00 00 00 00 00 00 00
     * 01 00 01 00 F6 0C 00 00 00 00 00 00 00 00 00 00
     * 01 00 01 00 F6 0C 00 00 00 00 00 00 00 00 00 00
     * 01 00 01 00 F6 0C 00 00 00 00 00 00 00 00 00 00
     * 01 00 01 00 5A 0B 00 00 00 00 00 00 00 00 00 00
     * 01 00 01 00 F6 0C 00 00 00 00 00 00 00 00 00 00
     * 01 00 01 00 F6 0C 00 00 00 00 00 00 00 00 00 00
     *
     */
    //TODO parse data
    hexdump(data,res,"KCMD_READ_DATA1");
    delete[] data;
  }

  return res;
}

int KReadData14(libusb_device_handle *handle)
{
  uint8_t* data = NULL;
  int res = KGenericCommand(handle, KCMD_READ_VERSIONS, 0x00, 20, 0x5C, &data);

  if (data != NULL)
  {
    //TODO parse data
    hexdump(data,res,"KCMD_READ_VERSIONS");
    delete[] data;
  }

  return res;
}

int KReadData22_1(libusb_device_handle *handle)
{
  uint8_t* data = NULL;
  int res = KGenericCommand(handle, KCMD_READ_DATA_PAGE, 0x01, 24, 0x80, &data);



  if (data != NULL)
  {
    // serial number as 0 terminated string?!
    hexdump(data,res,"KCMD_READ_DATA_PAGE 0x01");
    delete[] data;
  }

  return res;
}

int KReadP0Tables(libusb_device_handle *handle, libfreenect2::DepthPacketProcessor& depth_processor)
{
  uint8_t* data = NULL;
  int res = KGenericCommand(handle, KCMD_READ_DATA_PAGE, 0x02, 24, 0x1C0000, &data);

  if (data != NULL)
  {
    // PTables
    depth_processor.loadP0TablesFromCommandResponse(data, res);
    delete[] data;
  }

  return res;
}

int KReadDepthCameraParams(libusb_device_handle *handle)
{
  uint8_t* data = NULL;
  int res = KGenericCommand(handle, KCMD_READ_DATA_PAGE, 0x03, 24, 0x1C0000, &data);

  if (data != NULL)
  {
    if (res == sizeof(DepthCameraParams)) {
      DepthCameraParams* dp = (DepthCameraParams*)data;
      printf("depth camera intrinsic parameters: fx %f, fy %f, cx %f, cy %f\n",dp->fx,dp->fy,dp->cx,dp->cy);
      printf("depth camera radial distortion coeffs: k1 %f, k2 %f, p1 %f, p2 %f, k3 %f\n",dp->k1,dp->k2,dp->p1,dp->p2,dp->k3);
    }
    delete[] data;
  }

  return res;
}

int KReadCameraParams(libusb_device_handle *handle)
{
  uint8_t* data = NULL;
  int res = KGenericCommand(handle, KCMD_READ_DATA_PAGE, 0x04, 24, 0x1C0000, &data);

  if (data != NULL)
  {
    if (res == sizeof(CameraParams)) {
      CameraParams* cp = (CameraParams*)data;
      for(size_t i = 0; i < 25; ++i)
        std::cout << cp->intrinsics[i] << std::endl;
    }
    delete[] data;
  }

  return res;
}

int KReadStatus90000(libusb_device_handle *handle)
{
  uint8_t* data = NULL;
  int res = KGenericCommand(handle, KCMD_READ_STATUS, 0x90000, 24, 0x04, &data);

  if (data != NULL)
  {
    //TODO parse data
    uint32_t* data32 = (uint32_t*) data;
    uint32_t value = *data32;
    printf("Received status KCMD_READ_STATUS (0x90000): %d\n", value);
    delete[] data;
  }

  return res;
}

int KReadStatus100007(libusb_device_handle *handle)
{
  uint8_t* data = NULL;
  int res = KGenericCommand(handle, KCMD_READ_STATUS, 0x100007, 24, 0x04, &data);

  if (data != NULL)
  {
    //TODO parse data
    uint32_t* data32 = (uint32_t*) data;
    uint32_t value = *data32;
    printf("Received status KCMD_READ_STATUS (0x100007): %d\n", value);
    delete[] data;
  }

  return res;
}

int KInitStreams(libusb_device_handle *handle)
{
  int res = KGenericCommand(handle, KCMD_INIT_STREAMS, 0x00, 20);

  return res;
}

int KSetStreamStatus(libusb_device_handle *handle, KStreamStatus KStreamStatus)
{
  int res = KGenericCommand(handle, KCMD_SET_STREAMING, KStreamStatus, 24);

  printf("Set stream status success: %s\n", (KStreamStatus == KSTREAM_ENABLE ? "Enable" : "Disable"));

  return res;
}

int KSetModeStatus(libusb_device_handle *handle, KModeStatus KModeStatus)
{
  int res = KGenericCommand(handle, KCMD_SET_MODE, KModeStatus, 36);

  printf("Set mode status success: %s\n", (KModeStatus == KMODE_ENABLE ? "Enable" : "Disable"));

  return res;
}

int KReadData26(libusb_device_handle *handle)
{
  uint8_t* data = NULL;
  int res = KGenericCommand(handle, KCMD_READ_COUNT, 0x00, 24, 0x10, &data);

  if (data != NULL)
  {
    //TODO parse data
    uint16_t* data16 = (uint16_t*) data;
    int numValues = res / 2;
    for (int i = 0; i < numValues; i++)
    {
      uint16_t value = *data16;
      data16++;
      printf("Received status KCMD_READ_COUNT (0x00) %d: %d\n", i, value);
    }
    delete[] data;
  }

  return res;
}

void InitKinect(libusb_device_handle *handle)
{
  if (NULL == handle)
    return;

  printf("running kinect...\n");
  int r;
  uint8_t bmRequestType;
  uint8_t bRequest;
  uint16_t wValue;
  uint16_t wIndex;
  unsigned char* data = NULL;
  uint16_t wLength = 0;
  unsigned int timeout = 1000;

  bmRequestType = LIBUSB_RECIPIENT_DEVICE;
  bRequest = LIBUSB_SET_ISOCH_DELAY;
  wValue = 40; //ms?
  wIndex = 0;
  wLength = 0;
  data = NULL;

  printf("Control transfer 1 - set isoch delay\n");
  r = libusb_control_transfer(handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
  if (r < 0)
  {
    perr("Control transfer error: %d\n", r);
  }

  bmRequestType = LIBUSB_RECIPIENT_DEVICE;
  bRequest = LIBUSB_REQUEST_SET_SEL;
  wValue = 0;
  wIndex = 0;
  wLength = 6;
  unsigned char seldata[] =
  { 0x55, 0x00, 0x55, 0x00, 0x00, 0x00 };

  printf("Control transfer 2 - set sel u1/u2\n");
  r = libusb_control_transfer(handle, bmRequestType, bRequest, wValue, wIndex, seldata, wLength, timeout);
  if (r < 0)
  {
    perr("Control transfer error: %d\n", r);
  }

  printf("\nSetting interface alt setting...\n");
  r = KSetStreamingInterfaceStatus(handle, KSTREAM_DISABLE);
  if (r != LIBUSB_SUCCESS)
  {
    perr("   Failed: %d.\n", r);
  }

  bmRequestType = LIBUSB_RECIPIENT_DEVICE;
  bRequest = LIBUSB_REQUEST_SET_FEATURE;
  wValue = U1_ENABLE;
  wIndex = 0;
  wLength = 0;

  printf("Control transfer 3 - enable u1\n");
  r = libusb_control_transfer(handle, bmRequestType, bRequest, wValue, wIndex, NULL, wLength, timeout);
  if (r < 0)
  {
    perr("Control transfer error: %d\n", r);
  }

  bmRequestType = LIBUSB_RECIPIENT_DEVICE;
  bRequest = LIBUSB_REQUEST_SET_FEATURE;
  wValue = U2_ENABLE;
  wIndex = 0;
  wLength = 0;

  printf("Control transfer 4 - enable u2\n");
  r = libusb_control_transfer(handle, bmRequestType, bRequest, wValue, wIndex, NULL, wLength, timeout);
  if (r < 0)
  {
    perr("Control transfer error: %d\n", r);
  }

  printf("Control transfer 5 - set feature 768\n");
  KSetSensorStatus(handle, KSENSOR_DISABLE);

  printf("Kinect init done\n\n");
}

void RunKinect(libusb_device_handle *handle, libfreenect2::DepthPacketProcessor& depth_processor)
{
  if (NULL == handle)
    return;
    
  printf("running kinect...\n");
  int r;

  r = KSetSensorStatus(handle, KSENSOR_ENABLE);

  r = KReadData02(handle);

  r = KReadData14(handle);

  r = KReadData22_1(handle);

  r = KReadDepthCameraParams(handle);

  r = KReadP0Tables(handle, depth_processor);

  r = KReadCameraParams(handle);

  r = KReadStatus90000(handle);

  r = KInitStreams(handle);

  r = KSetStreamingInterfaceStatus(handle, KSTREAM_ENABLE);

  r = KReadStatus90000(handle);

  r = KSetStreamStatus(handle, KSTREAM_ENABLE);

}

void CloseKinect(libusb_device_handle *handle)
{
  printf("closing kinect...\n");
  int r;
  r = KSetSensorStatus(handle, KSENSOR_DISABLE);
}

bool shutdown = false;

void sigint_handler(int s)
{
  shutdown = true;
}

//-------------------
//-------------------
//-------------------
//-------------------
//-------------------

// ------ ofProtonect

//-------------------
//-------------------
//-------------------
//-------------------

ofProtonect::ofProtonect(){
    frame_listener = NULL;
    depth_iso_transfersPtr = NULL;
    rgb_bulk_transfersPtr = NULL;
    handle = NULL;
    dev = NULL;
    rgb_bulk_transfers = NULL;
    rgb_packet_stream_parser = NULL;
    depth_processor = NULL;
    depth_iso_transfers = NULL;
    depth_packet_stream_parser = NULL;
    rgb_processor = NULL;
    bOpened = false;
    
    ofAddListener(ofEvents().exit, this, &ofProtonect::exit);
}

int ofProtonect::openKinect(std::string binpath){
  if( bOpened ){
    closeKinect();
  }
  cmd_seq = 0;

  uint16_t vid = 0x045E;
  uint16_t pid[2] = {0x02d8, 0x02C4};
  uint16_t mi = 0x00;

  bool debug_mode = false;

  uint8_t bus;

  int r;

  const struct libusb_version* version;
  version = libusb_get_version();
  printf("Using libusbx v%d.%d.%d.%d\n\n", version->major, version->minor, version->micro, version->nano);

  r = libusb_init(NULL);
  if (r < 0) return r;

  libusb_set_debug(NULL, debug_mode ? LIBUSB_LOG_LEVEL_DEBUG : LIBUSB_LOG_LEVEL_INFO);

  for(int i = 0; i < 2; i++){
      printf("Trying to open device %04X:%04X...\n", vid, pid[i]);

  handle = NULL;
  int tryCount = 4;
  if (handle == NULL){
    while(tryCount > 0 && handle == NULL){
                handle = libusb_open_device_with_vid_pid(NULL, vid, pid[i]);
        tryCount--;
        ofSleepMillis(100);
        
        if( handle ){
          libusb_reset_device(handle);
          ofSleepMillis(100);
                  handle = libusb_open_device_with_vid_pid(NULL, vid, pid[i]);
                }
            }
      }
      if(handle !=  NULL){
        break;
      }
  }
  
    if( handle == NULL ){
        perr("ofProtonect::openKinect  Failed. - handle is NULL\n");
        return -1;
    }

  dev = libusb_get_device(handle);
  bus = libusb_get_bus_number(dev);
  /*
   struct libusb_device_descriptor dev_desc;

   printf("\nReading device descriptor:\n");
   CALL_CHECK(libusb_get_device_descriptor(dev, &dev_desc));
   printf("            length: %d\n", dev_desc.bLength);
   printf("      device class: %d\n", dev_desc.bDeviceClass);
   printf("               S/N: %d\n", dev_desc.iSerialNumber);
   printf("           VID:PID: %04X:%04X\n", dev_desc.idVendor, dev_desc.idProduct);
   printf("         bcdDevice: %04X\n", dev_desc.bcdDevice);
   printf("   iMan:iProd:iSer: %d:%d:%d\n", dev_desc.iManufacturer, dev_desc.iProduct, dev_desc.iSerialNumber);
   printf("          nb confs: %d\n", dev_desc.bNumConfigurations);
   */

  r = libusb_get_device_speed(dev);
  if ((r < 0) || (r > 4))
    r = 0;
  printf("             speed: %s\n", speed_name[r]);

  int active_cfg = -5;
  r = libusb_get_configuration(handle, &active_cfg);

  printf("active configuration: %d, err: %d", active_cfg, r);
  int configId = 1;
  if (active_cfg != configId)
  {
    printf("Setting config: %d\n", configId);
    r = libusb_set_configuration(handle, configId);
    if (r != LIBUSB_SUCCESS)
    {
      perr("  Can't set configuration. Error code: %d (%s)\n", r, libusb_error_name(r));
    }
  }

  int iface = 0;
  printf("\nClaiming interface %d...\n", iface);
  r = libusb_claim_interface(handle, iface);
  if (r != LIBUSB_SUCCESS)
  {
    perr("   Failed: %d.\n", r);
  }

  iface = 1;
  printf("\nClaiming interface %d...\n", iface);
  r = libusb_claim_interface(handle, iface);
  if (r != LIBUSB_SUCCESS)
  {
    perr("   Failed: %d.\n", r);
  }

  InitKinect(handle);

  // install signal handler now
  signal(SIGINT,sigint_handler);
  shutdown = false;

  usb_loop.start();
  
  //INITIALIZE OBJECTS
  
  frame_listener = new libfreenect2::FrameListener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
  rgb_bulk_transfers = new libfreenect2::usb::BulkTransferPool(handle, 0x83);
  
  rgb_processor = new libfreenect2::ofRGBPacketProcessor();
  rgb_packet_stream_parser = new libfreenect2::RgbPacketStreamParser(rgb_processor);

  rgb_processor->setFrameListener(frame_listener);

  rgb_bulk_transfers->allocate(50, 0x4000);
  rgb_bulk_transfers->setCallback(rgb_packet_stream_parser);
  rgb_bulk_transfers->enableSubmission();
  rgb_bulk_transfersPtr = rgb_bulk_transfers;

  depth_iso_transfers = new libfreenect2::usb::IsoTransferPool(handle, 0x84);
  
  depth_processor = new libfreenect2::CpuDepthPacketProcessor();
  depth_processor->setFrameListener(frame_listener);
  depth_processor->load11To16LutFromFile((binpath + "11to16.bin").c_str());
  depth_processor->loadXTableFromFile((binpath + "xTable.bin").c_str());
  depth_processor->loadZTableFromFile((binpath + "zTable.bin").c_str());

  depth_packet_stream_parser = new libfreenect2::DepthPacketStreamParser(depth_processor); 

  size_t max_packet_size = libusb_get_max_iso_packet_size(dev, 0x84);
  std::cout << "iso max_packet_size: " << max_packet_size << std::endl;

  depth_iso_transfers = new libfreenect2::usb::IsoTransferPool(handle, 0x84);
  depth_iso_transfers->allocate(80, 8, max_packet_size);
  depth_iso_transfers->setCallback(depth_packet_stream_parser);
  depth_iso_transfers->enableSubmission();
  depth_iso_transfersPtr = depth_iso_transfers;

  r = libusb_get_device_speed(dev);
  if ((r < 0) || (r > 4))
    r = 0;
  printf("             speed: %s\n", speed_name[r]);

  RunKinect(handle, *depth_processor);

  rgb_bulk_transfers->submit(10);
  depth_iso_transfers->submit(60);

  r = libusb_get_device_speed(dev);
  if ((r < 0) || (r > 4))
    r = 0;
  printf("             speed: %s\n", speed_name[r]);
  
  bOpened = true;

    return 0;
}

void ofProtonect::exit(ofEventArgs & args){
    closeKinect();
}

void ofProtonect::updateKinect(ofPixels & rgbPixels, ofFloatPixels & depthPixels){
    if( bOpened ){
        
        ofLogVerbose() << " updateKinect " << endl;
        frame_listener->waitForNewFrame(frames);
        ofLogVerbose() << " frame in " << endl;
        
        if( bOpened == false )return;

        libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
        libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
        libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];
        
        rgbPixels.setFromPixels(rgb->data, rgb->width, rgb->height, 3);
        depthPixels.setFromPixels((float *)depth->data, ir->width, ir->height, 1);

        frame_listener->release(frames);
  }
}

//TODO: get the deletion shutdown of objects ordered right
int ofProtonect::closeKinect(){

 if( bOpened ){

    bOpened = false;
    shutdown = true;
    
    rgb_bulk_transfers->disableSubmission();
    depth_iso_transfers->disableSubmission();

    rgb_bulk_transfers->cancel();
    depth_iso_transfers->cancel();
    

    CloseKinect(handle);

    // wait for all transfers to cancel
    // TODO: better implementation
    libfreenect2::this_thread::sleep_for(libfreenect2::chrono::seconds(2));

    rgb_bulk_transfers->deallocate();
    depth_iso_transfers->deallocate();

    int iface = 0;
    printf("Releasing interface %d...\n", iface);
    libusb_release_interface(handle, iface);

    iface = 1;
    printf("Releasing interface %d...\n", iface);
    libusb_release_interface(handle, iface);

    printf("Closing device...\n");
    libusb_close(handle);

    usb_loop.stop();

    libusb_exit(NULL);
    
    
    if(frame_listener != NULL){
        delete frame_listener;
        frame_listener = NULL;
    }
    
    if(rgb_bulk_transfers != NULL){
        delete rgb_bulk_transfers;
        rgb_bulk_transfers = NULL;
    }
    
    if(depth_iso_transfers != NULL){
        delete depth_iso_transfers;
        depth_iso_transfers = NULL;
    }
    
    if(depth_processor != NULL){
        delete depth_processor;
        depth_processor = NULL;
    }

    if(rgb_processor != NULL){
        delete rgb_processor;
        rgb_processor = NULL;
    }
    
    if(rgb_packet_stream_parser != NULL){
        delete rgb_packet_stream_parser;
        rgb_packet_stream_parser = NULL;
    }
    
    if(depth_packet_stream_parser != NULL){
        delete depth_packet_stream_parser;
        depth_packet_stream_parser = NULL;
    }
    
    
     
  }
  return 0;
}
