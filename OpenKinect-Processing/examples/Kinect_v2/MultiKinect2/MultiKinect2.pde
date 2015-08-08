/*
Thomas Sanchez Lengeling
http://codigogenerativo.com/

How to use multiple Kinects v2 in the same sketch.
Should work up n number of Kinects v2 connected to the USB 3.0 port.

https://github.com/shiffman/OpenKinect-for-Processing
http://shiffman.net/p5/kinect/
*/

import org.openkinect.processing.*;

Kinect2 kinect2a;
Kinect2 kinect2b;

void setup() {
  size(1536, 848, P2D);

  kinect2a = new Kinect2(this);
  kinect2a.initDepth();
  kinect2a.initVideo();
  kinect2a.initIR();

  kinect2b = new Kinect2(this);
  kinect2b.initDepth();
  kinect2b.initVideo();
  kinect2b.initIR();

  //Start tracking each kinect
  kinect2a.initDevice(0); //index 0
  kinect2b.initDevice(1); //index 1

  background(0);
}

void draw() {
  background(0);

  image(kinect2a.getDepthImage(), 0, 0);
  image(kinect2a.getIrImage(), 512, 0);
  image(kinect2a.getVideoImage(), 512*2, 0, 512, 424);

  image(kinect2b.getDepthImage(), 0, 424);
  image(kinect2b.getIrImage(), 512, 424);
  image(kinect2b.getVideoImage(), 512*2, 424, 512, 424);
}