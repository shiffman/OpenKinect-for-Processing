// Thomas Sanchez Lengeling
// Multi Kinect with all features 
// Example with 2 Kinects connected on a single Mac

// https://github.com/shiffman/OpenKinect-for-Processing

import org.openkinect.freenect.*;
import org.openkinect.processing.*;

Kinect kinectA;
Kinect kinectB;

float deg;

boolean ir = false;
boolean colorDepth = false;

int numDevices = 0;

//index to change the current device changes
int deviceIndex = 0;

void setup() {
  size(1280, 1040);
  kinectA = new Kinect(this);
  
  //get the number of current connected devices
  int numDevicesA = kinectA.numDevices();

  numDevices =  numDevicesA;
  println("number of Kinect v1 devices  "+numDevicesA);
  
  //set the KinectA to index USB 0
  kinectA.setDevice(0);
  
  kinectA.startDepth();
  kinectA.startVideo();
  //kinect.setIR(ir);
  kinectA.setColorDepth(colorDepth);
  
  //set the KinectB to index USB 1
  kinectB = new Kinect(this);
  kinectB.setDevice(1);
  
  kinectB.startDepth();
  kinectB.startVideo();
  //kinect.setIR(ir);
  kinectB.setColorDepth(colorDepth);
  
  deg = kinectB.getTilt();
  // kinect.tilt(deg);
}


void draw() {
  background(0);
  image(kinectA.getVideoImage(), 0, 0);
  image(kinectA.getDepthImage(), 640, 0);
  
  image(kinectB.getVideoImage(), 0, 520);
  image(kinectB.getDepthImage(), 640, 520);
  
  fill(255);
  text("Device Count: " +numDevices + "  " +
      "Current Device Index Device: "+deviceIndex, 10, 492);
  
  text("Press 'd' to enable/disable depth    " + 
    "Press 'r' to enable/disable rgb image   " + 
    "Press 'i' to enable/disable IR image  " + 
    "Press 'c' to enable/disable color depth image  " + 
    "UP and DOWN to tilt camera   " +
    "Framerate: " + int(frameRate), 10, 515);
}

void keyPressed() {
  if(key == '1'){
    deviceIndex = 0;
  }
  
  if(key == '2'){
    deviceIndex = 1;
  }
  
  
  if (key == 'd') {
    kinectA.toggleDepth();
  } else if (key == 'r') {
    kinectA.toggleVideo();
  } else if (key == 'i') {
    ir = !ir;
    kinectA.setIR(ir);
  } else if (key == 'c') {
    colorDepth = !colorDepth;
    kinectA.setColorDepth(colorDepth);
  } else if (key == CODED) {
    if (keyCode == UP) {
      deg++;
    } else if (keyCode == DOWN) {
      deg--;
    }
    deg = constrain(deg, 0, 30);
    kinectA.tilt(deg);
  }
}