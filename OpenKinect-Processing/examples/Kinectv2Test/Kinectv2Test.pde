// Daniel Shiffman
// Testing Kinect v2

// https://github.com/shiffman/OpenKinect-for-Processing
// http://shiffman.net/p5/kinect/


import org.openkinect.freenect2.*;
import org.openkinect.processing.Kinect2;



Kinect2 openKinect2;

void setup() {
  size(1536, 424);
  openKinect2 = new Kinect2(this);
  openKinect2.open();

  background(0);
}


void draw() {
  background(0);

  PImage depth = openKinect2.getDepthImage();
  image(depth, 0, 0);

  PImage ir = openKinect2.getIrImage();
  image(ir, 512, 0);

  PImage rgb = openKinect2.getColorImage();
  image(rgb, 512*2, 0, 512, 424);
}