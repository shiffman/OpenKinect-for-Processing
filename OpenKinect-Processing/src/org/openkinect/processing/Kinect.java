package org.openkinect.processing;


import java.lang.reflect.Method;
import java.nio.ShortBuffer;

import org.openkinect.Context;
import org.openkinect.Device;

import processing.core.PApplet;
import processing.core.PImage;

public class Kinect extends Thread {

	public static PApplet p5parent;
	Method kinectEventMethod;

	boolean running = false;

	Context context;
	Device device;
	int w = 640;
	int h = 480;
	RGBImage kimg;// = new KImage();
	DepthImage dimg;

	public Kinect(PApplet _p) {
		p5parent = _p;
		try {
			kinectEventMethod = p5parent.getClass().getMethod("kinectEvent",
					new Class[] { Kinect.class });
		} catch (Exception e) {
			//System.out.println("You are missing the kinectEvent() method. " + e);
		}
		//p5parent.registerDraw(this);
	}

	public void start() {
		start(0);
	}
	
	public void start(int num) {
		context = Context.getContext();
		if(context.devices() < 1)
		{
			System.out.println("No Kinect devices found.");
		}
		device = context.getDevice(num);
		kimg = new RGBImage(p5parent);
		dimg = new DepthImage(p5parent);
		running = true;
		super.start();
	}

	public int[] getRawDepth() {
		ShortBuffer sb = dimg.getRawData();
		//int[] depth = new int[sb.capacity()];
		int[] depth = new int[w*h];

		// This is inefficent, but I think it's easier for Processing users to have an int array?
		if (sb != null) {
			for (int i = 0; i < depth.length; i++) {
				depth[i] = sb.get(i);
				//System.out.println(sb.get(i));

			}
		} else {
			// Instead of null, filling it with 0's
			for (int i = 0; i < depth.length; i++) depth[i] = 0;
		}		
		return depth;
	}

	public void tilt(float deg)
	{
		device.tilt(deg);
	}

	public void processDepthImage(boolean b) {
		dimg.enableImage(b);
	}

	public void enableDepth(boolean b) {
		if (b) device.depth(dimg);
		else device.depth(null);
	}

	public void enableRGB(boolean b) {
		device.color(null);
		kimg.setIR(!b);
		if (b) device.color(kimg);
		//else device.color(null);
	}

	public void enableIR(boolean b) {
		device.color(null);
		kimg.setIR(b);
		if (b) device.color(kimg,true);
		//else device.color(null);
	}

	public float getVideoFPS() {
		return kimg.getFPS();
	}

	public float getDepthFPS() {
		return dimg.getFPS();
	}

	public PImage getVideoImage() {
		return kimg.img;
		//return kimg.imgCopy;
	}

	public PImage getDepthImage() {
		return dimg.img;
	}


	/**
	 * This method should only be called internally by Thread.start().
	 */
	public void run() {
		while (running) {
			boolean b = context.processEvents();
			try {
				Thread.sleep(2);
			} catch (InterruptedException e) {
				// e.printStackTrace();
			}
		}
		/*try {
			// call the method with this object as the argument!
			frameEventMethod.invoke(p5parent, new Object[] { this });

		} catch (Exception e) {
			err("Could not invoke the \"frameEvent()\" method for some reason.");
			e.printStackTrace();
			frameEventMethod = null;
		} */

	}

	/* Stops the client thread.  You don't really need to do this ever.
	 */  
	public void quit() {
		System.out.println("quitting");
		device.color(null);
		device.depth(null);
		running = false;  // Setting running to false ends the loop in run()
		//interrupt();      // In case the thread is waiting. . .
	}



}