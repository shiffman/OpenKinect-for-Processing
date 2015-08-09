package org.openkinect.processing;

import processing.core.PApplet;

public class TestIt extends PApplet {



	private Kinect kinect;

	public static void main(String[] args) {
		PApplet.main(new String[] { "org.openkinect.processing.TestIt"});
	}
	
	public void settings() {
		size(1280, 700);
	}

	public void setup() {
		kinect = new Kinect(this);
		kinect.initDepth();
		kinect.initVideo();
		kinect.enableColorDepth(true);
	}

	public void draw() {
		background(0);
		image(kinect.getVideoImage(), 0, 0);
		 image(kinect.getDepthImage(), 640, 0);
	}

}
