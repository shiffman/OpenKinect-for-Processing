// Daniel Shiffman
// Kinect Point Cloud example

// https://github.com/shiffman/OpenKinect-for-Processing
// http://shiffman.net/p5/kinect/

// Broken!

import org.openkinect.processing.*;

// Kinect Library object
Kinect2 kinect2;

// Angle for rotation
float a = 3.1;

void setup() {
  // Rendering in P3D
  size(800, 600, P3D);
  kinect2 = new Kinect2(this);
  kinect2.startDepth();
  kinect2.start();

}

void draw() {

  background(0);

  // Get the raw depth as array of integers
  int[] depth = kinect2.getRawDepth();

  // We're just going to calculate and draw every 4th pixel (equivalent of 160x120)
  int skip = 1;

  // Translate and rotate
  translate(width/2, height/2, 50);
  rotateY(a);

  fill(255);
  stroke(255);
  for (int x = 0; x < kinect2.depthWidth; x++) {
    for (int y = 0; y < kinect2.depthHeight; y++) {
      int offset = x + y * kinect2.depthWidth;
      
      pushMatrix();
      PVector point = depthToPointCloudPos(x, y,  depth[offset]);
      translate(point.x, point.y, point.z);
      // Draw a point
      point(0, 0);
      popMatrix();
      
    }
  }

  // Rotate
  //a += 0.0015f;
}
 //<>//
PVector depthToPointCloudPos(int x, int y, float depthValue) {
  PVector point = new PVector();
  point.z = (depthValue);// / (1.0f); // Convert from mm to meters
  point.x = (x - CameraParams.cx) * point.z / CameraParams.fx;
  point.y = (y - CameraParams.cy) * point.z / CameraParams.fy;
  return point;
}