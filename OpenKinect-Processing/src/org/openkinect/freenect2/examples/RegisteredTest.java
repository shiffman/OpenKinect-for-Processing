package org.openkinect.freenect2.examples;

import org.openkinect.freenect2.*;
import org.openkinect.processing.Kinect2;

import processing.core.*;


public class RegisteredTest extends PApplet{
	
	Kinect2 openKinect2;

	public static void main(String[] args) {
		PApplet.main(new String[] { "org.openkinect.freenect2.examples.RegisteredTest"});
	}

	public void settings() {
		size(512*3, 424*2);
	}
	
	
	public void setup(){
		openKinect2 = new Kinect2(this);
		openKinect2.start();
	}
	
	
	public void draw(){
		background(0);
		
		PImage depth = openKinect2.getDepthImage();
		image(depth, 0, 0);
		
		PImage ir = openKinect2.getIrImage();
		image(ir, 512, 0);
		
		PImage rgb = openKinect2.getVideoImage();
		image(rgb, 512*2, 0, 512, 424);
		
		PImage unregistered = openKinect2.getUndistoredImage();
		image(unregistered, 0, 424, 512, 424);
		
		PImage registered = openKinect2.getRegisteredImage();
		image(registered, 512, 424, 512, 424);
	}
	
}
