package examples;

import openKinectv2.*;
import processing.core.*;

public class FrameTest extends PApplet{
	
	OpenKinectv2 openKinect2;

	public void setup(){
		size(512*3, 424, P3D);
		openKinect2 = new OpenKinectv2(this);
		openKinect2.open();
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
