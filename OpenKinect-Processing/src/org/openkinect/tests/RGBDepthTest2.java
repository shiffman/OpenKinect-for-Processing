package org.openkinect.tests;

import org.openkinect.processing.Kinect2;

import processing.core.*;

public class RGBDepthTest2 extends PApplet{

	Kinect2 kinect2;

	public static void main(String[] args) {
		PApplet.main(new String[] { "org.openkinect.tests.RGBDepthTest2"});
	}

	public void settings() {
		size(512*2, 424*2, P2D);		
	}
	
	public void setup() {
		kinect2 = new Kinect2(this);
		// Start all data
		kinect2.initVideo();
		kinect2.initDepth();
		kinect2.initIR();
		kinect2.initRegistered();
		kinect2.initDevice();
	}


	public void draw() {
		background(0);
		//image(kinect2.getVideoImage(), 0, 0, width, height);
		image(kinect2.getDepthImage(), 0, 0);
		image(kinect2.getVideoImage(), kinect2.depthWidth, 0, kinect2.colorWidth*0.25f, kinect2.colorHeight*0.25f);
		image(kinect2.getIrImage(), 0, kinect2.depthHeight);
		image(kinect2.getRegisteredImage(), kinect2.depthWidth, kinect2.depthHeight);
		fill(255);
		text("Framerate: " + (int)(frameRate), 10, 515);
	}

}
