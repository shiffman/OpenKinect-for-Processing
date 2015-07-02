# Open Kinect for Processing 

The library for first generation kinects is (hopefully temporarily) broken.  However, it can work if you install libusb separately.

```
$ brew install libusb 
```

This assumes you've installed [http://brew.sh/](homebrew).

# libfreenect2 library for Processing

Thanks to @ThomasLengeling, there is now a Processing Kinect v2 library here!  These two are in the process of being integrated into a single library.

## Requirements

- USB 3.0 Host Controller (NEC Corporation USB 3.0, Intel Chipset)
- Currently only for Mac OS X
- Processing 2.2.1
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