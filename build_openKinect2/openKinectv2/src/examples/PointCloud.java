package examples;

import openKinectv2.*;
import processing.core.*;

public class PointCloud extends PApplet{
	
	OpenKinectv2 openKinect2;

	public static void main(String[] args) {
		PApplet.main(new String[] { "examples.PointCloud"});
	}

	public void settings() {
		size(1024, 768, P2D);
	}
	
	
	public void setup(){
		openKinect2 = new OpenKinectv2(this);
		openKinect2.open();
		
		background(0);
	}
	
	
	public void draw(){
		background(0);
		
		PImage depth = openKinect2.getDepthImage();
		image(depth, 0, 0);
		
		//raw depth data, ints from 0 -4500, distance values
		int [] rawData = openKinect2.getRawDepthData();

	}
	
}
