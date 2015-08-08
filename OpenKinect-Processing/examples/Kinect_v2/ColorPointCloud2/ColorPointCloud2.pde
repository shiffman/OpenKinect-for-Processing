// Thomas Sanchez Lengeling //<>//
// Kinect 3d Point Cloud example with different color types.

// https://github.com/shiffman/OpenKinect-for-Processing
// http://shiffman.net/p5/kinect/

import java.nio.*;
import org.openkinect.processing.Kinect2;


Kinect2 kinect2;

float angle = 3.141594;
float scaleValue = 50;

//change color of the point cloud
int drawState = 1;


//openGL
PGL pgl;
PShader sh;

int  vertLoc;
int  colorLoc;


void setup() {
  size(1280, 1020, P3D);

  kinect2 = new Kinect2(this);
  // Start all data
  kinect2.initDepth();
  kinect2.initIR();
  kinect2.initVideo();
  kinect2.initRegistered();
  kinect2.initDevice();

  //start shader
  //shader usefull to add post-render effects to the point cloud
  sh = loadShader("frag.glsl", "vert.glsl");
  smooth(16);
}


void draw() {
  background(0);

  //draw all the Kinect v2 frames
  image(kinect2.getDepthImage(), 0, 0, 320, 240);
  image(kinect2.getIrImage(), 320, 0, 320, 240);
  image(kinect2.getVideoImage(), 320*2, 0, 320, 240);
  image(kinect2.getRegisteredImage(), 320*3, 0, 320, 240);
  fill(255);


  //rotate the scene
  pushMatrix();
  translate(width/2, height/2, scaleValue);
  rotateY(angle);
  stroke(255);

  int vertData = kinect2.depthWidth * kinect2.depthHeight;

  //obtain the XYZ camera positions based on the depth data
  FloatBuffer depthPositions = kinect2.getDepthBufferPositions();
  
  /*obtain individual positions use depthPositions.get(i)
  
  for(int i = 0; i < u < 512 * 424; i++){
     float val = depthPositions.get(i);
  }
  */

  //obtain the color information as IntBuffers
  IntBuffer irData = kinect2.getIrColorBuffer();
  IntBuffer registeredData = kinect2.getRegisteredColorBuffer();
  IntBuffer depthData      = kinect2.getDepthColorBuffer();


  pgl = beginPGL();
  sh.bind();

  vertLoc  = pgl.getAttribLocation(sh.glProgram, "vertex");
  colorLoc = pgl.getAttribLocation(sh.glProgram, "color");

  pgl.enableVertexAttribArray(vertLoc);
  pgl.enableVertexAttribArray(colorLoc);

  pgl.vertexAttribPointer(vertLoc, 3, PGL.FLOAT, false, 0, depthPositions);

  //change color of the point cloud depending on the depth, ir and color+depth information.
  switch(drawState){
   case 0:
    pgl.vertexAttribPointer(colorLoc, 4, PGL.UNSIGNED_BYTE, true, 0, depthData);
   break;
   case 1:
    pgl.vertexAttribPointer(colorLoc, 4, PGL.UNSIGNED_BYTE, true, 0, irData);
   break;
   case 2:
    pgl.vertexAttribPointer(colorLoc, 4, PGL.UNSIGNED_BYTE, true, 0, registeredData);
   break;
  }

  //draw the XYZ depth camera points
  pgl.drawArrays(PGL.POINTS, 0, vertData);

  //clean up the vertex buffers
  pgl.disableVertexAttribArray(vertLoc);
  pgl.disableVertexAttribArray(colorLoc);

  sh.unbind();
  endPGL();

  popMatrix();

  text("Framerate: " + (int)(frameRate), 10, 515);
}

void keyPressed() {

  if(key == '1'){
   drawState = 0;
  }

  if(key == '2'){
    drawState = 1;
  }
  if(key == '3'){
    drawState = 2;
  }

  if (key == 'a') {
    angle += 0.1;
    println("angle "+angle);
  }

  if (key == 's') {
    angle -= 0.1;
    println("angle "+angle);
  }

  if (key == 'z') {
    scaleValue +=5;
    println("scaleValue "+scaleValue);
  }


  if (key == 'x') {
    scaleValue -=5;
    println("scaleValue "+scaleValue);
  }
}