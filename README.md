# Open Kinect for Processing 

![screenshot](https://pbs.twimg.com/media/CI8UO0RXAAApEX9.png:medium)

This repository is for for using the Microsoft Kinect sensor with [Processing](http://processing.org).  It uses the open source drivers from the [libfreenect](https://github.com/OpenKinect/libfreenect) project. 

For more, read this [online tutorial](http://shiffman.net/p5/kinect/) or check out the [javadocs](http://shiffman.net/p5/kinect/javadoc/).

Some important notes:
* This is current Mac OS X only! [I'd like this to change](https://github.com/shiffman/OpenKinect-for-Processing/issues/13).
* The current release only supports 1st edition kinects.  Support for kinect v2 is [coming soon](https://github.com/shiffman/OpenKinect-for-Processing/issues/16)!
* This new version is yet to be updated for the Processing contributions manager.  [Download here](https://github.com/shiffman/OpenKinect-for-Processing/releases).

# Info about using kinect v2

## libfreenect2 library for Processing

Thanks to @ThomasLengeling, there is now a Processing Kinect v2 library here!  Some notes below.  This will eventually be integrated into the [release](https://github.com/shiffman/OpenKinect-for-Processing/releases) but for the moment you'll have to do it manually.

## Requirements

- USB 3.0 Host Controller (NEC Corporation USB 3.0, Intel Chipset)
- Currently only for Mac OS X
- Kinect v2
 
## Installation

- Install [brew](http://brew.sh/) for Mac OS X
- Install the following libraries in the Terminal using brew for Mac

```      
 brew update 
 brew tap homebrew/science
 brew install wget nasm jpeg-turbo
 brew tap homebrew/versions
 brew install glfw3
```
 
- Copy openKinect2 folder into your processing library folder.
- Enjoy.
