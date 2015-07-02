package shiffman.test;



import java.nio.ByteBuffer;
import java.nio.DoubleBuffer;
import java.util.ArrayList;
import java.util.List;

import org.openkinect.freenect.Context;
import org.openkinect.freenect.DepthHandler;
import org.openkinect.freenect.Device;
import org.openkinect.freenect.FrameMode;
import org.openkinect.freenect.Freenect;
import org.openkinect.freenect.LogHandler;
import org.openkinect.freenect.LogLevel;
import org.openkinect.freenect.TiltStatus;
import org.openkinect.freenect.VideoHandler;
import org.openkinect.freenect.Freenect.*;
import org.openkinect.processing.Kinect;

import com.sun.jna.Pointer;
import com.sun.jna.ptr.PointerByReference;

import processing.core.PApplet;
import processing.core.PImage;

public class FreenectTest extends PApplet {
	Kinect kinect;

	PImage depth;
	PImage video;


	public static void main(String args[]) {
		PApplet.main(new String[] { "shiffman.test.FreenectTest" });
	}

	public void settings() {
		size(1280, 480);
	}

	public void setup() {
		println("Kinect camera test\n");
		kinect = new Kinect(this);
		kinect.startDepth();
		kinect.startVideo();
	}

	public void depthEvent(Kinect k) {
		depth = kinect.getDepthImage();
	}

	public void videoEvent(Kinect k) {
		video = kinect.getVideoImage();
	}

	public void draw() {
		background(0);
		if (depth != null) {
			image(depth,0,0);
		}
		if (video != null) {
			image(video,640,0);
		}
	}

	public void keyPressed() {
		if (key == 'd') {
			kinect.toggleDepth();
		} else if (key == 'v') {
			kinect.toggleVideo();
		}

	}


}
