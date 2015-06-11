package examples;

import openKinectv2.*;
import processing.core.*;


public class DepthTest extends PApplet{
	
	OpenKinectv2 openKinect2;

	public void setup(){
		size(500, 500);
		openKinect2 = new OpenKinectv2(this);
		openKinect2.open();
	}
	
	
	public void draw(){
		
	}
	
}
