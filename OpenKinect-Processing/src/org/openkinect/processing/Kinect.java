package org.openkinect.processing;


import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.nio.ShortBuffer;

import org.openkinect.freenect.Context;
import org.openkinect.freenect.DepthFormat;
import org.openkinect.freenect.DepthHandler;
import org.openkinect.freenect.Device;
import org.openkinect.freenect.FrameMode;
import org.openkinect.freenect.Freenect;
import org.openkinect.freenect.VideoFormat;
import org.openkinect.freenect.VideoHandler;

import processing.core.PApplet;
import processing.core.PConstants;
import processing.core.PImage;

public class Kinect {

	public static PApplet p5parent;

	Method depthEventMethod;
	Method videoEventMethod;

	boolean irEnabled = false;

	PImage depthImage;
	PImage videoImage;
	ShortBuffer rawDepthBuffer;
	int[] rawDepth;

	Context context;
	Device device;

	int w = 640;
	int h = 480;

	boolean irMode = false;
	boolean colorDepthMode = false;

	boolean depthEnabled = false;
	boolean videoEnabled = false;

	int currentDeviceIndex = 0;

	boolean started = false;

	public Kinect(PApplet _p) {
		p5parent = _p;

		try {
			depthEventMethod = p5parent.getClass().getMethod("depthEvent",new Class[] { Kinect.class });
		} catch (Exception e) {
			//System.out.println("You are missing the depthEventMethod() method. " + e);
			depthEventMethod = null;
		}

		try {
			videoEventMethod = p5parent.getClass().getMethod("videoEvent",new Class[] { Kinect.class });
		} catch (Exception e) {
			//System.out.println("You are missing the depthEventMethod() method. " + e);
			videoEventMethod = null;
		}

		depthImage = p5parent.createImage(w, h, PConstants.RGB);
		videoImage = p5parent.createImage(w, h, PConstants.RGB);

		context = Freenect.createContext();
		if(numDevices() < 1) {
			System.err.println("No Kinect devices found.");
		}
		//start(0);
	}

	public int numDevices() {
		return context.numDevices();		
	}

	public void setDevice(int n) {
		currentDeviceIndex = n;
	}

	void start() {
		started = true;
		device = context.openDevice(currentDeviceIndex);
	}

	public int[] getRawDepth() {
		if (rawDepth == null) {
			rawDepth = new int[w*h];
		}
		// This is inefficent, but I think it's easier for Processing users to have an int array?
		if (rawDepthBuffer != null) {
			for (int i = 0; i < rawDepth.length; i++) {
				rawDepth[i] = rawDepthBuffer.get(i);
			}
		}
		return rawDepth;
	}

	public void stopDepth() {
		device.stopDepth();
		depthEnabled = false;
	}

	public void stopVideo() {
		device.stopVideo();
		videoEnabled = false;
	}

	public void toggleDepth() {
		depthEnabled = !depthEnabled;
		if (depthEnabled) {
			startDepth();
		} else {
			stopDepth();
		}
	}

	public void toggleVideo() {
		videoEnabled = !videoEnabled;
		if (videoEnabled) {
			startVideo();
		} else {
			stopVideo();
		}
	}

	public void startDepth() {

		if (!started) {
			start();
		}
		depthEnabled = true;
		device.setDepthFormat(DepthFormat.D11BIT);
		final Kinect ref = this;
		device.startDepth(new DepthHandler() {
			public void onFrameReceived(FrameMode mode, ByteBuffer frame, int timestamp) {
				rawDepthBuffer = frame.asShortBuffer();
				DepthImage.data(rawDepthBuffer, depthImage, colorDepthMode);
				if (depthEventMethod != null) {
					try {
						depthEventMethod.invoke(p5parent,  new Object[] { ref } );
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			}
		});
	}

	public void startVideo() {
		if (!started) {
			start();
		}
		videoEnabled = true;
		final Kinect ref = this;
		if (irMode) {
			device.setVideoFormat(VideoFormat.IR_8BIT);
		} else {
			device.setVideoFormat(VideoFormat.RGB);
		}

		device.startVideo(new VideoHandler() {
			public void onFrameReceived(FrameMode mode, ByteBuffer frame, int timestamp) {
				RGBImage.data(frame, videoImage, irMode);
				if (videoEventMethod != null) {
					try {
						videoEventMethod.invoke(p5parent,  new Object[] { ref } );
					} catch (Exception e) {
						e.printStackTrace();
					}
				}
			}
		});
	}

	public void tilt(float deg) {
		device.setTiltAngle(deg);
	}

	public float getTilt() {
		return (float) device.getTiltAngle();
	}

	public void setIR(boolean b) {
		irMode = b;
	}

	public void setColorDepth(boolean b) {
		colorDepthMode = b;
	}

	public PImage getDepthImage() {
		return depthImage;
	}
	public PImage getVideoImage() {
		return videoImage;
	}
}