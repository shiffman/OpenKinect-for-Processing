// Thomas Sanchez Lengeling
// openGL Point Cloud Example.

// https://github.com/shiffman/OpenKinect-for-Processing
// http://shiffman.net/p5/kinect/

import java.nio.*;
import org.openkinect.freenect.*;
import org.openkinect.processing.*;

// Kinect Library object
Kinect kinect;

// Angle for rotation
float a = 0;

//openGL object and shader
PGL     pgl;
PShader sh;

//VBO buffer location in the GPU
int vertexVboId;

void setup() {
  // Rendering in P3D
  size(800, 600, P3D);
  kinect = new Kinect(this);
  kinect.initDepth();

  //load shaders
  sh = loadShader("frag.glsl", "vert.glsl");

  PGL pgl = beginPGL();

  IntBuffer intBuffer = IntBuffer.allocate(1);
  pgl.genBuffers(1, intBuffer);

  //memory location of the VBO
  vertexVboId = intBuffer.get(0);

  endPGL();
}

void draw() {

  background(0);

  image(kinect.getDepthImage(), 0, 0, 320, 240);

  pushMatrix();
  translate(width/2, height/2, 50);
  scale(150);
  rotateY(a);

  
  //data size times 3 for each XYZ coordinate
  int vertData = kinect.width * kinect.height;

  //begin openGL calls and bind the shader
  pgl = beginPGL();
  sh.bind();
  
  FloatBuffer depthPositions =  kinect.getDephToWorldPositions();


  //obtain the vertex location in the shaders.
  //useful to know what shader to use when drawing the vertex positions
  vertexVboId = pgl.getAttribLocation(sh.glProgram, "vertex");

  pgl.enableVertexAttribArray(vertexVboId);

  //bind vertex positions to the VBO
  {
    pgl.bindBuffer(PGL.ARRAY_BUFFER, vertexVboId);
    // fill VBO with data
    pgl.bufferData(PGL.ARRAY_BUFFER, Float.BYTES * vertData *3, depthPositions, PGL.DYNAMIC_DRAW);
    // associate currently bound VBO with shader attribute
    pgl.vertexAttribPointer(vertexVboId, 3, PGL.FLOAT, false, Float.BYTES * 3, 0 );
  }

  // unbind VBOs
  pgl.bindBuffer(PGL.ARRAY_BUFFER, 0);

  //draw the point buffer as a set of POINTS
  pgl.drawArrays(PGL.POINTS, 0, vertData);

  //disable the vertex positions
  pgl.disableVertexAttribArray(vertexVboId);

  //finish drawing
  sh.unbind();
  endPGL();

  popMatrix();

  fill(255, 0, 0);
  text(frameRate, 50, 50);

  // Rotate
  // a += 0.015f;
}