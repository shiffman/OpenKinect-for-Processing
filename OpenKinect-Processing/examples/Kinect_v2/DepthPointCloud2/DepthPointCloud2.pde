// Daniel Shiffman
// Kinect Point Cloud example

// https://github.com/shiffman/OpenKinect-for-Processing
// http://shiffman.net/p5/kinect/

// Broken!

import org.openkinect.processing.*;

// Kinect Library object
Kinect2 kinect2;

// Angle for rotation
float a = 0;

// We'll use a lookup table so that we don't have to repeat the math over and over
float[] depthLookUp = new float[2048];

void setup() {
  // Rendering in P3D
  size(800, 600, P3D);
  kinect2 = new Kinect2(this);
  kinect2.startDepth();
  kinect2.start();

  // Lookup table for all possible depth values (0 - 2047)
  for (int i = 0; i < depthLookUp.length; i++) {
    depthLookUp[i] = rawDepthToMeters(i);
  }
}

void draw() {

  background(0);

  // Get the raw depth as array of integers
  int[] depth = kinect2.getRawDepth();

  // We're just going to calculate and draw every 4th pixel (equivalent of 160x120)
  int skip = 1;

  // Translate and rotate
  translate(width/2, height/2, -50);
  rotateY(a);

  fill(255);
  stroke(255);
  for (int x = 0; x < kinect2.depthWidth; x+=5) {
    for (int y = 0; y < kinect2.depthHeight; y+=5) {
      int offset = x + y * kinect2.depthHeight;
      
      pushMatrix();
      PVector point = depthToPointCloudPos(x, y,  depth[offset]);
      translate(point.x, point.y, point.z);
      // Draw a point
      point(0, 0);
      popMatrix();
      
    }
  }

  // Rotate
  a += 0.015f;
}

// These functions come from: http://graphics.stanford.edu/~mdfisher/Kinect.html
float rawDepthToMeters(int depthValue) { //<>//
  if (depthValue < 2047) {
    return (float)(1.0 / ((double)(depthValue) * -0.0030711016 + 3.3309495161));
  }
  return 0.0f;
}

PVector depthToPointCloudPos(int x, int y, float depthValue) {
  PVector point = new PVector();
  point.z = (depthValue) / (1000.0f); // Convert from mm to meters
  point.x = (x - CameraParams.cx) * point.z / CameraParams.fx;
  point.y = (y - CameraParams.cy) * point.z / CameraParams.fy;
  return point;
}