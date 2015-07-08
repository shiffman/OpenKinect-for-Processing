// Daniel Shiffman
// All features test

// https://github.com/shiffman/OpenKinect-for-Processing
// http://shiffman.net/p5/kinect/

import org.openkinect.freenect.*;
import org.openkinect.freenect2.*;
import org.openkinect.freenect2.examples.*;
import org.openkinect.processing.*;

Kinect2 kinect2;


void setup() {
  size(1024, 848, P2D);

  kinect2 = new Kinect2(this);
  // Start all data
  kinect2.start();
}


void draw() {
  background(0);
  image(kinect2.getVideoImage(), 0, 0, kinect2.colorWidth*0.267, kinect2.colorHeight*0.267);
  image(kinect2.getDepthImage(), kinect2.width, 0);
  image(kinect2.getIrImage(), 0, kinect2.height);
  image(kinect2.getRegisteredImage(), kinect2.width, kinect2.height);
  fill(255);
  text("Framerate: " + (int)(frameRate), 10, 515);
}