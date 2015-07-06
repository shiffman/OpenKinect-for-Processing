package org.openkinect.processing;

import processing.core.PApplet;

public class TestIt extends PApplet {



	private Kinect kinect;

	public static void main(String[] args) {
		PApplet.main(new String[] { "org.openkinect.processing.TestIt"});
	}

	public void setup() {
		
		kinect = new Kinect(this);

	}

	public void draw() {
		background(0);

	}

}
