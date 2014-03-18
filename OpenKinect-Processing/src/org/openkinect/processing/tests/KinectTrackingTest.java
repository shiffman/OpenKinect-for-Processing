package org.openkinect.processing.tests;

import processing.core.PApplet;
import processing.core.PFont;
import processing.core.PVector;

@SuppressWarnings("serial")

public class KinectTrackingTest extends PApplet {

	
	// Showing how we can farm all the kinect stuff out to a separate class
	KinectTracker tracker;

	public void setup() {
		size(640,520);
		KinectTracker.p = this;
		tracker = new KinectTracker();
	}



	public void draw() {
		background(255);
		
		// Run the tracking analysis
		tracker.track();
		// Show the image
		tracker.display();
		
		// Let's draw the raw location
		PVector v1 = tracker.getPos();
		fill(50,100,250,200);
		noStroke();
		ellipse(v1.x,v1.y,20,20);
		
		// Let's draw the "lerped" location
		PVector v2 = tracker.getLerpedPos();
		fill(100,250,50,200);
		noStroke();
		ellipse(v2.x,v2.y,20,20);
		
		// Display some info
		int t = tracker.getThreshold();
		fill(0);
		text("threshold: " + t + "    " +  "framerate: " + (int)frameRate,10,500);
	}
	
	public void keyPressed() {
		int t = tracker.getThreshold();
		if (key == CODED) {
			if (keyCode == UP) {
				t+=5;
				tracker.setThreshold(t);
			} else if (keyCode == DOWN) {
				t-=5;
				tracker.setThreshold(t);
			}
		}
	}
	
	public void stop() {
		tracker.quit();
		super.stop();
	}
	
	/**** Standard Run ****/
	static public void main(String _args[]) {
		//PApplet.main(new String[] { "--present", myClassName });
		PApplet.main(new String[] { myClassName });
	}

	protected static String myClassName = getQualifiedClassName();

	public static String getQualifiedClassName() {
		return new Exception().getStackTrace()[1].getClassName();
	}

}
