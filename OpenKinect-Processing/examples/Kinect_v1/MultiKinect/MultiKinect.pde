// Thomas Sanchez Lengeling //<>// //<>//
// Multi Kinect with all features
// Example connecting multiple Kinects v1 on a single Mac

// https://github.com/shiffman/OpenKinect-for-Processing
// http://shiffman.net/p5/kinect/

import org.openkinect.freenect.*;
import org.openkinect.processing.*;


ArrayList<Kinect> multiKinect;


boolean ir = false;
boolean colorDepth = false;

int numDevices = 0;

//index to change the current device changes
int deviceIndex = 0;

float deg = 0;

void setup() {
  size(1024, 720);

  //get the actual number of devices before creating them
  numDevices = Kinect.countDevices();
  println("number of Kinect v1 devices  "+numDevices);

  //creat the arraylist
  multiKinect = new ArrayList<Kinect>();

  //iterate though all the devices and activate them
  for (int i  = 0; i < numDevices; i++) {
    Kinect tmpKinect = new Kinect(this);
    tmpKinect.activateDevice(i);
    tmpKinect.initDepth();
    tmpKinect.initVideo();
    tmpKinect.enableColorDepth(colorDepth);

    multiKinect.add(tmpKinect);
  }

}


void draw() {
  background(0);

  //iterat though the array of kinects
  for (int i  = 0; i < multiKinect.size(); i++) {
    Kinect tmpKinect = (Kinect)multiKinect.get(i);

    //make the kinects capture smaller to fit the window
    image(tmpKinect.getVideoImage(), 0, 240*i, 320, 240);
    image(tmpKinect.getDepthImage(), 320, 240*i, 320, 240);
  }


  fill(255);
  text("Device Count: " +numDevices + "  \n" +
    "Current Index: "+deviceIndex, 660, 50, 150, 50);

  text(
    "Press 'i' to enable/disable between video image and IR image  \n" +
    "Press 'c' to enable/disable between color depth and gray scale depth \n" +
    "UP and DOWN to tilt camera : "+deg+"  \n" +
    "Framerate: " + int(frameRate), 660, 100, 280, 250);
}

void keyPressed() {
  if (key == '-') {
    if(deviceIndex > 0 && numDevices > 0){
      deviceIndex--;
      deg = multiKinect.get(deviceIndex).getTilt();
    }
  }

  if (key == '+') {
    if(deviceIndex < numDevices - 1){
      deviceIndex++;
      deg = multiKinect.get(deviceIndex).getTilt();
    }
  }


  if (key == 'i') {
    ir = !ir;
    multiKinect.get(deviceIndex).enableIR(ir);
  } else if (key == 'c') {
    colorDepth = !colorDepth;
      multiKinect.get(deviceIndex).enableColorDepth(colorDepth);
  } else if (key == CODED) {
    if (keyCode == UP) {
      deg++;
    } else if (keyCode == DOWN) {
      deg--;
    }
    deg = constrain(deg, 0, 30);
    multiKinect.get(deviceIndex).setTilt(deg);
  }
}