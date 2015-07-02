import org.openkinect.freenect.*;
import org.openkinect.processing.*;

Kinect kinect;

float deg = 15; // Start at 15 degrees

boolean ir = false;

void setup() {
  size(1280, 520);
  kinect = new Kinect(this);
  //kinect.start();
  kinect.startDepth();
  kinect.startVideo();
  kinect.enableIR(ir);
  kinect.tilt(deg);
}


void draw() {
  background(0);
  image(kinect.getVideoImage(), 0, 0);
  image(kinect.getDepthImage(), 640, 0);
  fill(255);
  text("Press 'd' to enable/disable depth    Press 'r' to enable/disable rgb image   Press 'i' to enable/disable IR image  UP and DOWN to tilt camera   Framerate: " + frameRate, 10, 515);
}

void keyPressed() {
  if (key == 'd') {
    kinect.toggleDepth();
  } else if (key == 'r') {
    kinect.toggleVideo();
  } else if (key == 'i') {
    ir = !ir;
    kinect.enableIR(ir);
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