 //<>//
import java.nio.*;
import org.openkinect.processing.Kinect2;


Kinect2 kinect2;

PGL pgl;
PShader sh;

int  vertLoc;
int  colorLoc;


float angle = 3.141594;
float scaleValue = 50;

//change color of the point cloud
int drawState = 1;


void setup() {
  size(1280, 1020, P3D); 

  kinect2 = new Kinect2(this);
  // Start all data
  kinect2.startDepth();
  kinect2.startIR();
  kinect2.startVideo();
  kinect2.startRegistered();
  kinect2.start();

  println(Integer.SIZE);

  sh = loadShader("frag.glsl", "vert.glsl");
  smooth(16);
}


void draw() {
  background(0);
  //image(kinect2.getVideoImage(), 0, 0, width, height);
  image(kinect2.getDepthImage(), 0, 0, 320, 240);
  image(kinect2.getIrImage(), 320, 0, 320, 240);
  image(kinect2.getVideoImage(), 320*2, 0, 320, 240);
  image(kinect2.getRegisteredImage(), 320*3, 0, 320, 240);
  fill(255);


  pushMatrix();
  translate(width/2, height/2, scaleValue);
  rotateY(angle);
  stroke(255);

  int vertData = kinect2.depthWidth * kinect2.depthHeight;

  FloatBuffer depthPositions = kinect2.getDepthBufferPositions();

  IntBuffer irData = kinect2.getIrColorBuffer();
  IntBuffer registeredData = kinect2.getRegisteredColorBuffer();
  IntBuffer depthData      = kinect2.getDepthColorBuffer();


  pgl = beginPGL();
  sh.bind();

  vertLoc  = pgl.getAttribLocation(sh.glProgram, "vertex");
  colorLoc = pgl.getAttribLocation(sh.glProgram, "color");

  //color for each POINT of the point cloud
  //sh.set("fragColor", 1.0f, 1.0f, 1.0f, 1.0f);

  pgl.enableVertexAttribArray(vertLoc);
  pgl.enableVertexAttribArray(colorLoc);


  //pgl.vertexAttribPointer(vertLoc, 3, PGL.FLOAT, false, 3 * (Float.SIZE/8), pointCloudBuffer);
  pgl.vertexAttribPointer(vertLoc, 3, PGL.FLOAT, false, 0, depthPositions);
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



  pgl.drawArrays(PGL.POINTS, 0, vertData);

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