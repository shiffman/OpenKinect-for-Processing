import openKinectv2.*;
OpenKinectv2 openKinect2;

void setup() {
  size(1024, 768);
  openKinect2 = new OpenKinectv2(this);
  openKinect2.open();
}


void draw() {
  background(0);

  PImage depth = openKinect2.getDepthImage();
  image(depth, 0, 0);
  
  
  //get raw depth data, int distance values from 0 - 4500 
  int rawDepth []  = openKinect2.getRawDepthData();
  
}
