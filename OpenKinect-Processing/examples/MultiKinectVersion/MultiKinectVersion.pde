
//Thomas Sanchez Lengeling
//http://codigogenerativo.com/

//Multiple Kinect versions on a single sketch.
//https://github.com/shiffman/OpenKinect-for-Processing
//http://shiffman.net/p5/kinect/

import org.openkinect.processing.*;

Kinect kinect;
Kinect2 kinect2;

float deg;


void setup() {
  size(1536, 848);

  //v2
  kinect2 = new Kinect2(this);
  kinect2.initDepth();
  kinect2.initVideo();
  kinect2.initIR();
  kinect2.initDevice();

  //v1
  kinect = new Kinect(this);
  kinect.initDepth();
  kinect.initVideo();
  kinect.enableColorDepth(false);
  deg = kinect.getTilt();
}


void draw() {
  background(0);

  //v2
  PImage depth = kinect2.getDepthImage();
  image(depth, 0, 0);

  PImage ir = kinect2.getIrImage();
  image(ir, 512, 0);

  PImage rgb = kinect2.getVideoImage();
  image(rgb, 512*2, 0, 512, 424);

  //v1 getDepthImage
  image(kinect.getDepthImage(), 0, 424, 512, 424);
  image(kinect.getVideoImage(), 512*2, 424, 512, 424);
}


//with key UP or DOWM tilt the Kinect v1
void keyPressed() {
  if (key == CODED) {
    if (keyCode == UP) {
      deg++;
      deg = constrain(deg, 0, 30);
      kinect.setTilt(deg);
    } else if (keyCode == DOWN) {
      deg--;
      deg = constrain(deg, 0, 30);
      kinect.setTilt(deg);
    }
  }
  
}