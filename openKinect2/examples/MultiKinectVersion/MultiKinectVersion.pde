import org.openkinect.freenect.*;
import org.openkinect.processing.*;

import openKinectv2.*;

Kinect       kinect;

OpenKinectv2 openKinect2;

void setup() {
  size(1536, 848);
  
  //v2
  openKinect2 = new OpenKinectv2(this);
  openKinect2.open();
  
  //v1
  kinect = new Kinect(this);
  kinect.startDepth();
  kinect.startVideo();
  kinect.setColorDepth(false);
}


void draw() {
  background(0);

  //v2
  PImage depth = openKinect2.getDepthImage();
  image(depth, 0, 0);

  PImage ir = openKinect2.getIrImage();
  image(ir, 512, 0);

  PImage rgb = openKinect2.getColorImage();
  image(rgb, 512*2, 0, 512, 424);
  
  //v1 getDepthImage
  image(kinect.getDepthImage(), 0, 424, 512, 424);
  image(kinect.getVideoImage(), 512*2, 424, 512, 424);
}
