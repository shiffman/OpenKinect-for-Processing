# Open Kinect for Processing 

![screenshot](https://pbs.twimg.com/media/CI8UO0RXAAApEX9.png:medium)

This repository is for for using the Microsoft Kinect sensor with [Processing](http://processing.org).  It uses the open source drivers from the [libfreenect](https://github.com/OpenKinect/libfreenect) and [libfreenect2](https://github.com/OpenKinect/libfreenect2) projects. 

### Documentation and tutorial

For more, read this [online tutorial](http://shiffman.net/p5/kinect/) or check out the [javadocs](http://shiffman.net/p5/kinect/reference).

## Kinect v1 Requirements
- Kinect v1 (supported versions??)
- Kinect v1 power adapter

####  Linux

Requirements?
```sudo apt-get install libusb-1.0-0 libusb-1.0-0-dev```

#### Windows.

 - Use [Zadig](http://zadig.akeo.ie/) to install the libusbK driver for each device you wish to use. This includes installing the libusbK for all the kinect v1 drivers listed on Zadig GUI.
 - If you got the error message 'Isochronous transfer error: 1', you should try downgrade the libusbK using [Older Zadig Version](http://zadig.akeo.ie/downloads/zadig_2.0.1.exe)

## Kinect v2 Requirements

- USB 3.0 Host Controller (NEC Corporation USB 3.0, Intel Chipset)
- Minimum Mac OS X 10.9
- Windows 8 64 bit support
- Kinect v2
 
#### Kinect v2 additional software
- For Windows 8, Follow the the [installation of the libusk driver.](https://github.com/OpenKinect/libfreenect2#libusbk-driver)
  - First install the Kinect v2 Windows SDK then change the USB 3.0 drivers properties to libusbK using Zadig.
- No longer need to external install  [Turbo Jpeg for Mac](http://sourceforge.net/projects/libjpeg-turbo/). 
