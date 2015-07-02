// Daniel Shiffman
// Depth thresholding example

// https://github.com/shiffman/OpenKinect-for-Processing
// http://shiffman.net/p5/kinect/


import org.openkinect.freenect.*;
import org.openkinect.processing.*;

Kinect kinect;

float deg;

boolean ir = false;
boolean colorDepth = false;

void setup() {
  size(1280, 520, P2D);
  kinect = new Kinect(this);
  kinect.startDepth();
  kinect.startVideo();
  //kinect.setIR(ir);
  kinect.setColorDepth(colorDepth);
  
  deg = kinect.getTilt();
  // kinect.tilt(deg);
}


void draw() {
  background(0);
  image(kinect.getVideoImage(), 0, 0);
  image(kinect.getDepthImage(), 640, 0);
  fill(255);
  text("Press 'd' to enable/disable depth    " + 
    "Press 'r' to enable/disable rgb image   " + 
    "Press 'i' to enable/disable IR image  " + 
    "Press 'c' to enable/disable color depth image  " + 
    "UP and DOWN to tilt camera   " + 
    "Framerate: " + int(frameRate), 10, 515);
}

void keyPressed() {
  if (key == 'd') {
    kinect.toggleDepth();
  } else if (key == 'r') {
    kinect.toggleVideo();
  } else if (key == 'i') {
    ir = !ir;
    kinect.setIR(ir);
  } else if (key == 'c') {
    colorDepth = !colorDepth;
    kinect.setColorDepth(colorDepth);
  } else if (key == CODED) {
    if (keyCode == UP) {
      deg++;
    } else if (keyCode == DOWN) {
      deg--;
    }
    deg = constrain(deg, 0, 30);
    kinect.tilt(deg);
  }
}