package examples;

import openKinectv2.*;
import processing.core.*;

public class MultipleKinects extends PApplet{
	
	OpenKinectv2 openKinect20;
	OpenKinectv2 openKinect21;
	
	public void setup(){
		size(512*3, 424*2, P2D);
		
		openKinect20 = new OpenKinectv2(this);
		openKinect20.printDevices();
		
		openKinect21 = new OpenKinectv2(this);
		openKinect21.printDevices();
		
		
		//System.out.println(num+" "+serialNumber);
		openKinect20.open(0);
		openKinect21.open(1);
		
		background(0);
	}
	
	
	public void draw(){
		background(0);
		
		image(openKinect20.getDepthImage(), 0, 0);
		image(openKinect20.getIrImage(), 512, 0);
		image(openKinect20.getColorImage(), 512*2, 0, 512, 424);
		
		image(openKinect21.getDepthImage(), 0, 424);
		image(openKinect21.getIrImage(), 512, 424);
		image(openKinect21.getColorImage(), 512*2, 424, 512, 424);
	}
}