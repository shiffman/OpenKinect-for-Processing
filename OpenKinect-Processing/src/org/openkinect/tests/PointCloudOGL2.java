package org.openkinect.tests;

import java.nio.FloatBuffer;

import org.openkinect.processing.Kinect2;

import processing.core.*;
import processing.opengl.PGL;
import processing.opengl.PShader;

public class PointCloudOGL2 extends PApplet{

	Kinect2 kinect2;
	
	PGL pgl;
	PShader sh;

	int  vertLoc;

	public static void main(String[] args) {
		PApplet.main(new String[] { "org.openkinect.tests.PointCloudOGL2"});
	}

	public void settings() {
		size(512*2, 424*2, P3D);		
	}
	
	public void setup() {
		kinect2 = new Kinect2(this);
		// Start all data
		kinect2.initDepth();
		kinect2.initIR();
		kinect2.initDevice();
		
		//System.out.println("Working Directory = " + System.getProperty("user.dir"));
		String dir = System.getProperty("user.dir")+"/src/org/openkinect/tests/data/";
		sh = loadShader( dir+"frag.glsl", dir+"vert.glsl");
	}


	public void draw() {
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
		// FIXME: Processing 3 switched to using an offset as the last parameter
		// "error: incompatible types: FloatBuffer cannot be converted to int"
		//pgl.vertexAttribPointer(vertLoc, 3, PGL.FLOAT, false, 0, depthPositions);
		pgl.drawArrays(PGL.POINTS, 0, vertData);

		pgl.disableVertexAttribArray(vertLoc);

		sh.unbind(); 
		endPGL();
		

		text("Framerate: " + (int)(frameRate), 10, 515);
	}

}