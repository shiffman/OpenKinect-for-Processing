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

//VBO buffer location in the GPU
int vertexVboId;
int colorVboId;

int vertLoc;
int colorLoc;

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

  //create VBO
  PGL pgl = beginPGL();

  // allocate buffer big enough to get all VBO ids back
  IntBuffer intBuffer = IntBuffer.allocate(2);
  pgl.genBuffers(2, intBuffer);

  //memory location of the VBO
  vertexVboId = intBuffer.get(0);
  colorVboId = intBuffer.get(1);

  endPGL();
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

  //obtain the XYZ camera positions based on the depth data
  FloatBuffer depthPositions = kinect2.getDepthBufferPositions();

  /*obtain individual positions use depthPositions.get(i)
   for(int i = 0; i < u < 512 * 424; i++){
   float val = depthPositions.get(i);
   }
   */

  //obtain the color information as IntBuffers
  IntBuffer irData         = kinect2.getIrColorBuffer();
  IntBuffer registeredData = kinect2.getRegisteredColorBuffer();
  IntBuffer depthData      = kinect2.getDepthColorBuffer();

  pgl = beginPGL();
  sh.bind();

  //send the the vertex positions (point cloud) and color down the render pipeline
  //positions are render in the vertex shader, and color in the fragment shader
  vertLoc = pgl.getAttribLocation(sh.glProgram, "vertex");
  pgl.enableVertexAttribArray(vertLoc);

  //enable drawing to the vertex and color buffer
  colorLoc = pgl.getAttribLocation(sh.glProgram, "color");
  pgl.enableVertexAttribArray(colorLoc);
  
  int vertData = kinect2.depthWidth * kinect2.depthHeight;

  //vertex
  {
    pgl.bindBuffer(PGL.ARRAY_BUFFER, vertexVboId);
    // fill VBO with data
    pgl.bufferData(PGL.ARRAY_BUFFER, Float.BYTES * vertData * 3, depthPositions, PGL.DYNAMIC_DRAW);
    // associate currently bound VBO with shader attribute
    pgl.vertexAttribPointer(vertLoc, 3, PGL.FLOAT, false, Float.BYTES * 3, 0);
  }


  //color
  //change color of the point cloud depending on the depth, ir and color+depth information.
  switch(drawState) {
  case 0:
    pgl.bindBuffer(PGL.ARRAY_BUFFER, colorVboId);
    // fill VBO with data
    pgl.bufferData(PGL.ARRAY_BUFFER, Integer.BYTES * vertData * 3, depthData, PGL.DYNAMIC_DRAW);
    // associate currently bound VBO with shader attribute
    pgl.vertexAttribPointer(colorLoc, 4, PGL.UNSIGNED_BYTE, false, Byte.SIZE, 0);
    break;
  case 1:
    pgl.bindBuffer(PGL.ARRAY_BUFFER, colorVboId);
    // fill VBO with data
    pgl.bufferData(PGL.ARRAY_BUFFER,  Integer.SIZE * vertData, irData, PGL.DYNAMIC_DRAW);
    // associate currently bound VBO with shader attribute
    pgl.vertexAttribPointer(colorLoc, 4, PGL.UNSIGNED_BYTE, false, Byte.SIZE, 0);
    break;
  case 2:
    pgl.bindBuffer(PGL.ARRAY_BUFFER, colorVboId);
    // fill VBO with data
    pgl.bufferData(PGL.ARRAY_BUFFER, Integer.SIZE * vertData, registeredData, PGL.DYNAMIC_DRAW);
    // associate currently bound VBO with shader attribute
    pgl.vertexAttribPointer(colorLoc, 4, PGL.UNSIGNED_BYTE, false, 0, 0);//Byte.SIZE, 0);
    break;
  }
 
  // unbind VBOs
  pgl.bindBuffer(PGL.ARRAY_BUFFER, 0);

  //draw the point cloud as a set of points
  pgl.drawArrays(PGL.POINTS, 0, vertData);

  //disable drawing
  pgl.disableVertexAttribArray(vertexVboId);
  pgl.disableVertexAttribArray(colorVboId);

  sh.unbind();
  endPGL();

  popMatrix();

  text("Framerate: " + (int)(frameRate), 10, 515);
}

void keyPressed() {

  if (key == '1') {
    drawState = 0;
  }

  if (key == '2') {
    drawState = 1;
  }
  if (key == '3') {
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