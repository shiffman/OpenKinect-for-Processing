/*
Thomas Sanchez Lengeling
http://codigogenerativo.com/

How to use multiple Kinects v2 in the same sketch.
Should work up n number of Kinects connected to the USB 3.0 port.

https://github.com/shiffman/OpenKinect-for-Processing
*/

import org.openkinect.processing.*;

Kinect2 kinect2a;
Kinect2 kinect2b;

void setup() {
  size(1536, 848, P2D);

  kinect2a = new Kinect2(this);
  kinect2a.startDepth();
  kinect2a.startVideo();
  kinect2a.startIR();
  
  kinect2b = new Kinect2(this);
  kinect2b.startDepth();
  kinect2b.startVideo();
  kinect2b.startIR();
  
  //Start tracking each kinect
  kinect2a.start(0); //index 0
  kinect2b.start(1); //index 1

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