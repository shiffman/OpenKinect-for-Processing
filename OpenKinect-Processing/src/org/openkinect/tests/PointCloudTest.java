package org.openkinect.tests;

import org.openkinect.processing.Kinect2;

import processing.core.*;

public class PointCloudTest extends PApplet{

	Kinect2 kinect2;

	public static void main(String[] args) {
		PApplet.main(new String[] { "org.openkinect.tests.PointCloudTest"});
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
	}


	public void draw() {
		background(0);
		//image(kinect2.getVideoImage(), 0, 0, width, height);
		image(kinect2.getDepthImage(), 0, 0, 320, 240);
		image(kinect2.getIrImage(), 0, 320, 320, 240);
		fill(255);
		
		float [] positions = kinect2.getDepthToCameraPositions();
		
		pushMatrix();
		translate(width/2, height/2, 50);
		rotateY(3.1f);
		stroke(255);
		fill(255);
		beginShape(POINTS);
		for(int i = 0; i < 424; i++){
			for(int j = 0; j < 512; j++){
				int index =  j + i*512;
				float x = positions[index*3 + 0];
				float y = positions[index*3 + 1];
				float z = positions[index*3 + 2];
				vertex(x, y, z);
			}
		}
		endShape();
		popMatrix();

		text("Framerate: " + (int)(frameRate), 10, 515);
	}

}
