// Daniel Shiffman
// Thomas Sanchez Lengeling
// Kinect Point Cloud example

// https://github.com/shiffman/OpenKinect-for-Processing
// http://shiffman.net/p5/kinect/

import org.openkinect.processing.*;

// Kinect Library object
Kinect2 kinect2;

// Angle for rotation
float rotationRads = 3.1;

void setup() {

  // Rendering in P3D
  size(800, 600, P3D);

  kinect2 = new Kinect2(this);
  kinect2.initDepth();
  kinect2.initDevice();

  smooth(16);
}


void draw() {
  background(0);

  // Translate and rotate
  pushMatrix();
  translate(width/2, height/2, 50);
  rotateY(rotationRads);

  // We're just going to calculate and draw every 2nd pixel
  int skip = 2;

  // Get the raw depth as array of integers
  int[] depth = kinect2.getRawDepth();

  stroke(255);
  beginShape(POINTS);
  for (int x = 0; x < kinect2.depthWidth; x += skip) {
    for (int y = 0; y < kinect2.depthHeight; y += skip) {
      int offset = x + y * kinect2.depthWidth;

      //calculte the x, y, z camera position based on the raw depth data
      PVector point = depthToPointCloudPos(x, y, depth[offset]);

      // Draw a point
      vertex(point.x, point.y, point.z);
    }
  }
  endShape();

  //pop the matrix scene, reset translation and rotation
  popMatrix();


  drawFrustum();
  
  // Rotate
  //rotationRads += 0.0015f;
}

//draw camera frustum
void drawFrustum(){

}

//calculate the xyz camera position based on the depth data
PVector depthToPointCloudPos(int x, int y, float depthValue) {
  PVector point = new PVector();
  point.z = (depthValue);// / (1.0f); // Convert from mm to meters
  point.x = (x - CameraParams.cx) * point.z / CameraParams.fx;
  point.y = (y - CameraParams.cy) * point.z / CameraParams.fy;
  return point;
}