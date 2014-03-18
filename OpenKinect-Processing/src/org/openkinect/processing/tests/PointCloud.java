package org.openkinect.processing.tests;

import org.openkinect.processing.Kinect;

import processing.core.PApplet;
import processing.core.PVector;

public class PointCloud extends PApplet {

	// Kinect Library object
	Kinect kinect;

	float a = 0;

	// Size of kinect image
	int w = 640;
	int h = 480;


	// We'll use a lookup table so that we don't have to repeat the math over and over
	float[] depthLookUp = new float[2048];

	public void setup() {
		size(800,600,P3D);
		kinect = new Kinect(this);
		kinect.start();
		kinect.enableDepth(true);
		// We don't need the grayscale image in this example
		// so this makes it more efficient
		kinect.processDepthImage(false);

		// Lookup table for all possible depth values (0 - 2047)
		for (int i = 0; i < depthLookUp.length; i++) {
			depthLookUp[i] = rawDepthToMeters(i);
		}
	}

	public void draw() {
		background(0);
		fill(255);
		textMode(SCREEN);
		text("Kinect FR: " + (int)kinect.getDepthFPS() + "\nProcessing FR: " + (int)frameRate,10,16);

		// Get the raw depth as array of integers
		int[] depth = kinect.getRawDepth();

		if (depth != null) {
			// We're just going to calculate and draw every 4th pixel (equivalent of 160x120)
			int skip = 4;

			// Translate and rotate
			translate(width/2,height/2,-50);
			rotateY(a);

			for(int x=0; x<w; x+=skip) {
				for(int y=0; y<h; y+=skip) {
					int offset = x+y*w;

					// Convert kinect data to world xyz coordinate
					int rawDepth = depth[offset];
					PVector v = depthToWorld(x,y,rawDepth);

					stroke(255);
					pushMatrix();
					// Scale up by 200
					float factor = 200;
					translate(v.x*factor,v.y*factor,factor-v.z*factor);
					// Draw a point
					point(0,0);
					popMatrix();


				}
			}
		}

		// Rotate
		a += 0.015f;
	}

	public void stop() {
		kinect.quit();
		super.stop();
	}

	public static void main(String _args[]) {
		PApplet.main(new String[] { org.openkinect.processing.tests.PointCloud.class.getName() });
	}


	// These functions come from: http://graphics.stanford.edu/~mdfisher/Kinect.html
	float rawDepthToMeters(int depthValue) {
		if (depthValue < 2047)	{
			return (float)(1.0 / ((double)(depthValue) * -0.0030711016 + 3.3309495161));
		}
		return 0.0f;
	}

	PVector depthToWorld(int x, int y, int depthValue) {

		final double fx_d = 1.0 / 5.9421434211923247e+02;
		final double fy_d = 1.0 / 5.9104053696870778e+02;
		final double cx_d = 3.3930780975300314e+02;
		final double cy_d = 2.4273913761751615e+02;

		PVector result = new PVector();
		double depth =  depthLookUp[depthValue];//rawDepthToMeters(depthValue);
		result.x = (float)((x - cx_d) * depth * fx_d);
		result.y = (float)((y - cy_d) * depth * fy_d);
		result.z = (float)(depth);
		return result;
	}








}
