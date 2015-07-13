import java.nio.FloatBuffer;
import org.openkinect.processing.Kinect2;


Kinect2 kinect2;

PGL pgl;
PShader sh;

int  vertLoc;


void setup() {
  size(1024, 768, P3D); 

  kinect2 = new Kinect2(this);
  // Start all data
  kinect2.startDepth();
  kinect2.startIR();
  kinect2.start();

  sh = loadShader("frag.glsl", "vert.glsl");
}


void draw() {
  background(0);
  //image(kinect2.getVideoImage(), 0, 0, width, height);
  image(kinect2.getDepthImage(), 0, 0, 320, 240);
  image(kinect2.getIrImage(), 0, 320, 320, 240);
  fill(255);

  FloatBuffer depthPositions = kinect2.getDepthBufferPositions();

  pgl = beginPGL();
  sh.bind();

  vertLoc = pgl.getAttribLocation(sh.glProgram, "vertex");

  //color for each POINT of the point cloud
  sh.set("fragColor", 1.0f, 1.0f, 1.0f, 1.0f);

  pgl.enableVertexAttribArray(vertLoc);

  //data size
  int vertData = kinect2.depthWidth * kinect2.depthHeight;

  //pgl.vertexAttribPointer(vertLoc, 3, PGL.FLOAT, false, 3 * (Float.SIZE/8), pointCloudBuffer);
  pgl.vertexAttribPointer(vertLoc, 3, PGL.FLOAT, false, 0, depthPositions);
  pgl.drawArrays(PGL.POINTS, 0, vertData);

  pgl.disableVertexAttribArray(vertLoc);

  sh.unbind(); 
  endPGL();


  text("Framerate: " + (int)(frameRate), 10, 515);
}