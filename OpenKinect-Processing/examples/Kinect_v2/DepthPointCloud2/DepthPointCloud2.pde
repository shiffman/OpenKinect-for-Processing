// Daniel Shiffman //<>//
// Thomas Sanchez Lengeling
// Kinect Point Cloud example

// https://github.com/shiffman/OpenKinect-for-Processing
// http://shiffman.net/p5/kinect/

import org.openkinect.processing.*;
import java.nio.FloatBuffer;

// Kinect Library object
Kinect2 kinect2;

// Angle for rotation
float a = 3.1;

//change render mode between openGL and CPU
int renderMode = 1;

//for openGL render
PGL pgl;
PShader sh;
int  vertLoc;


void setup() {

  // Rendering in P3D
  size(800, 600, P3D);

  kinect2 = new Kinect2(this);
  kinect2.initDepth();
  kinect2.initDevice();



  //load shaders
  sh = loadShader("frag.glsl", "vert.glsl");

  smooth(16);
}


void draw() {
  background(0);

  // Translate and rotate
  pushMatrix();
  translate(width/2, height/2, 50);
  rotateY(a);

  if (renderMode == 1) {

    // We're just going to calculate and draw every 2nd pixel
    int skip = 2;

    // Get the raw depth as array of integers
    int[] depth = kinect2.getRawDepth();

    stroke(255);
    beginShape(POINTS);
    for (int x = 0; x < kinect2.depthWidth; x+=skip) {
      for (int y = 0; y < kinect2.depthHeight; y+=skip) {
        int offset = x + y * kinect2.depthWidth;

        //calculte the x, y, z camera position based on the depth information
        PVector point = depthToPointCloudPos(x, y, depth[offset]);

        // Draw a point
        vertex(point.x, point.y, point.z);
      }
    }
    endShape();
  } else if ( renderMode == 2) {
    //get the depth data as a FloatBuffer
    FloatBuffer depthPositions = kinect2.getDepthBufferPositions();

    pgl = beginPGL();
    sh.bind();

    vertLoc = pgl.getAttribLocation(sh.glProgram, "vertex");

    //color for each POINT of the point cloud
    sh.set("fragColor", 1.0f, 1.0f, 1.0f, 1.0f);

    pgl.enableVertexAttribArray(vertLoc);

    //data size
    int vertData = kinect2.depthWidth * kinect2.depthHeight;

    pgl.vertexAttribPointer(vertLoc, 3, PGL.FLOAT, false, 0, depthPositions);
    pgl.drawArrays(PGL.POINTS, 0, vertData);

    pgl.disableVertexAttribArray(vertLoc);

    sh.unbind();
    endPGL();
  }
  popMatrix();

  fill(255, 0, 0);
  text(frameRate, 50, 50);

  // Rotate
  //a += 0.0015f;
}

void keyPressed() {

  if (key == '1') {
    renderMode = 1;
  }
  if (key == '2') {
    renderMode = 2;
  }
}

//calculte the xyz camera position based on the depth data
PVector depthToPointCloudPos(int x, int y, float depthValue) {
  PVector point = new PVector();
  point.z = (depthValue);// / (1.0f); // Convert from mm to meters
  point.x = (x - CameraParams.cx) * point.z / CameraParams.fx;
  point.y = (y - CameraParams.cy) * point.z / CameraParams.fy;
  return point;
}
