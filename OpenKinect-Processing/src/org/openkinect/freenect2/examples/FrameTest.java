package org.openkinect.freenect2.examples;

import org.openkinect.processing.Kinect2;

import processing.core.*;

public class FrameTest extends PApplet{
	
	Kinect2 openKinect2;

	public static void main(String[] args) {
		PApplet.main(new String[] { "org.openkinect.freenect2.examples.FrameTest"});
	}

	public void settings() {
		size(512*3, 424);
	}
	
	public void setup(){
		openKinect2 = new Kinect2(this);
		openKinect2.start();
		
		background(0);
	}
	
	
	public void draw(){
		background(0);
		
		PImage depth = openKinect2.getDepthImage();
		image(depth, 0, 0);
		
		PImage ir = openKinect2.getIrImage();
		image(ir, 512, 0);
		
		PImage color = openKinect2.getColorImage();
		image(color, 512*2, 0, 512, 424);
	}
	
}
