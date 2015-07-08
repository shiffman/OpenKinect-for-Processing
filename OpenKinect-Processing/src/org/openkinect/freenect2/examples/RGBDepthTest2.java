package org.openkinect.freenect2.examples;

import org.openkinect.processing.Kinect2;

import processing.core.*;

public class RGBDepthTest2 extends PApplet{

	Kinect2 kinect2;

	public static void main(String[] args) {
		PApplet.main(new String[] { "org.openkinect.freenect2.examples.RGBDepthTest2"});
	}

	boolean ir = false;

	public void setup() {
		size(1280, 520);
		kinect2 = new Kinect2(this);
		// Start all data
		kinect2.start();
	}


	public void draw() {
		background(0);
		image(kinect2.getVideoImage(), 0, 0);
		image(kinect2.getDepthImage(), 640, 0);
		fill(255);
		text("Press 'd' to enable/disable depth    " + 
				"Press 'r' to enable/disable rgb image   " + 
				"Press 'i' to enable/disable IR image  " + 
				"Press 'c' to enable/disable color depth image  " + 
				"UP and DOWN to tilt camera   " + 
				"Framerate: " + int(frameRate), 10, 515);
	}

}
