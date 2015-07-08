import org.openkinect.processing.*;

Kinect2 kinect2a;
Kinect2 kinect2b;

void setup() {
  size(512*3, 424*2, P2D);

  kinect2a = new Kinect2(this);
  kinect2a.printDevices();

  kinect2b = new Kinect2(this);
  kinect2b.printDevices();
  kinect2a.start(0);
  kinect2b.start(1);

  background(0);
}

void draw() {
  background(0);

  image(kinect2a.getDepthImage(), 0, 0);
  image(kinect2a.getIrImage(), 512, 0);
  image(kinect2a.getColorImage(), 512*2, 0, 512, 424);

  image(kinect2b.getDepthImage(), 0, 424);
  image(kinect2b.getIrImage(), 512, 424);
  image(kinect2b.getColorImage(), 512*2, 424, 512, 424);
}