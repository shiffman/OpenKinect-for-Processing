
import openKinectv2.*;


OpenKinectv2 openKinect2;

void setup() {
  size(512*3, 424*2);
  openKinect2 = new OpenKinectv2(this);
  openKinect2.open();
}


void draw() {
  background(0);

  PImage depth = openKinect2.getDepthImage();
  image(depth, 0, 0);

  PImage ir = openKinect2.getIrImage();
  image(ir, 512, 0);

  PImage rgb = openKinect2.getColorImage();
  image(rgb, 512*2, 0, 512, 424);

  PImage unregistered = openKinect2.getUndistoredImage();
  image(unregistered, 0, 424, 512, 424);

  PImage registered = openKinect2.getRegisteredImage();
  image(registered, 512, 424, 512, 424);
}

