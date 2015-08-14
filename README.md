# Open Kinect for Processing 

![screenshot](https://pbs.twimg.com/media/CI8UO0RXAAApEX9.png:medium)

This repository is for for using the Microsoft Kinect sensor with [Processing](http://processing.org).  It uses the open source drivers from the [libfreenect](https://github.com/OpenKinect/libfreenect) and [libfreenect2](https://github.com/OpenKinect/libfreenect2) projects. 

### Documentation and tutorial

For more, read this [online tutorial](http://shiffman.net/p5/kinect/) or check out the [javadocs](http://shiffman.net/p5/kinect/reference).


### Some important notes:
* Kinect v2 support is working (Thanks to @ThomasLengeling!!) but very much in progress. To do: [align APIs](https://github.com/shiffman/OpenKinect-for-Processing/issues/20), [make examples](https://github.com/shiffman/OpenKinect-for-Processing/issues/19), and [remove separate install for libturbojpeg](https://github.com/shiffman/OpenKinect-for-Processing/issues/12)
* This is current Mac OS X only! [I'd like this to change](https://github.com/shiffman/OpenKinect-for-Processing/issues/13).
* This new version is yet to be updated for the Processing contributions manager.  [Download here](https://github.com/shiffman/OpenKinect-for-Processing/releases).

## Kinect v1 Requirements
- Kinect v1 (supported versions??)
- Kinect v1 power adapter

## Kinect v1 for windows.

 - Use [Zadig](http://zadig.akeo.ie/) to install the libusbK driver for each device you wish to use. This includes installing the libusbK for all the kinect v1 drivers listed on Zadig GUI.

## Kinect v2 Requirements

- USB 3.0 Host Controller (NEC Corporation USB 3.0, Intel Chipset)
- Minimum Mac OS X 10.9
- Windows 8 64 bit support
- Kinect v2
 
#### Kinect v2 additional software
- For Windows 8, Follow the the [installation of the libusk driver.](https://github.com/OpenKinect/libfreenect2#libusbk-driver)
  - First install the Kinect v2 Windows SDK then change the USB 3.0 drivers properties to libusbK using Zadig.
- No longer need to external install  [Turbo Jpeg for Mac](http://sourceforge.net/projects/libjpeg-turbo/). 
