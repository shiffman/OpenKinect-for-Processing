/**
 * Open Kinect for Processing
 * A Mac OS X Kinect implementation using open source drivers (libfreenect).
 * https://github.com/shiffman/OpenKinect-for-Processing
 *
 * Copyright 2015 Daniel Shiffman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 * 
 * @author      Daniel Shiffman
 * @modified    July 3, 2015
 * @version     0.3a (3)
 */


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

	static PApplet p5parent;

	Method depthEventMethod;
	Method videoEventMethod;

	boolean irEnabled = false;

	PImage depthImage;
	PImage videoImage;
	ShortBuffer rawDepthBuffer;
	int[] rawDepth;

	Context context;
	Device device;

	public int width = 640;
	public int height = 480;

	boolean irMode = false;
	boolean colorDepthMode = false;

	boolean depthEnabled = false;
	boolean videoEnabled = false;

	int currentDeviceIndex = 0;

	boolean started = false;

	
	/**
	 * Kinect constructor, usually called in the setup() method in your sketch to
	 * initialize and start the library.
	 * 
	 * @param _p parent sketch (usually "this")
	 */
	
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

		depthImage = p5parent.createImage(width, height, PConstants.RGB);
		videoImage = p5parent.createImage(width, height, PConstants.RGB);

		context = Freenect.createContext();
		if(numDevices() < 1) {
			System.err.println("No Kinect devices found.");
		}
		//start(0);
	}

	/**
	 * Returns the number of Kinect devices detected
	 * 
	 * @return number of Kinect devices detected
	 */
	public int numDevices() {
		return context.numDevices();		
	}


	/**
	 * Set the active device
	 * 
	 * @param n index of which device to select
	 */
	public void setDevice(int n) {
		currentDeviceIndex = n;
	}
 
	// Called internally
	void start() {
		started = true;
		device = context.openDevice(currentDeviceIndex);		
	}

	/**
	 * Get the raw depth values from the Kinect.
	 * 
	 * @return the raw depth values (range: 0 - 2047) as int array
	 */
	public int[] getRawDepth() {
		if (rawDepth == null) {
			rawDepth = new int[width*height];
		}
		// This is inefficent, but I think it's easier for Processing users to have an int array?
		if (rawDepthBuffer != null) {
			for (int i = 0; i < rawDepth.length; i++) {
				rawDepth[i] = rawDepthBuffer.get(i);
			}
		}
		return rawDepth;
	}

	/**
	 * Stop getting depth from Kinect.
	 * 
	 */
	public void stopDepth() {
		device.stopDepth();
		depthEnabled = false;
	}

	/**
	 * Stop getting RGB video from Kinect.
	 * 
	 */
	public void stopVideo() {
		device.stopVideo();
		videoEnabled = false;
	}

	/**
	 * Toggle depth (start or stop depending on whether it's active or not.)
	 * 
	 */
	public void toggleDepth() {
		if (!depthEnabled) {
			startDepth();
		} else {
			stopDepth();
		}
	}


	/**
	 * Toggle RGB video (start or stop depending on whether it's active or not.)
	 * 
	 */
	public void toggleVideo() {
		if (!videoEnabled) {
			startVideo();
		} else {
			stopVideo();
		}
	}

	/**
	 * Start getting depth from Kinect (available as raw array or mapped to image)
	 * 
	 */
	public void startDepth() {

		if (!started) {
			start();
		}

		if (device != null  && !depthEnabled) {
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
	}


	/**
	 * Start getting RGB video from Kinect.
	 * 
	 */
	public void startVideo() {
		if (!started) {
			start();
		}
		if (device != null && !videoEnabled) {
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
	}

	/**
	 * Set the tilt angle of the Kinect.
	 * 
	 * @param deg the angle (in degrees, range 0-30).
	 */
	public void tilt(float deg) {
		if (device != null) {
			device.setTiltAngle(deg);
		}
	}

	/**
	 * Get the tilt angle of the Kinect.
	 * 
	 * @return the angle
	 */
	public float getTilt() {
		if (device != null) {
			return (float) device.getTiltAngle();
		} else {
			return 0;
		}
	}

	/**
	 * Enable IR image (instead of RGB video)
	 * 
	 * @param b true to turn it on, false to turn it off
	 */
	public void setIR(boolean b) {
		// If nothing has changed let's not do anything
		if (irMode == b) {
			return;
		}
		irMode = b;
		if (videoEnabled) {
			stopVideo();
		}
		if (irMode) {
			device.setVideoFormat(VideoFormat.IR_8BIT);
		} else {
			device.setVideoFormat(VideoFormat.RGB);
		}
		if  (!videoEnabled) {
			startVideo();
		}
	}

	/**
	 * Enable mapping depth values to color image (instead of grayscale)
	 * 
	 * @param b true to turn it on, false to turn it off
	 */	
	public void setColorDepth(boolean b) {
		colorDepthMode = b;
	}

	/**
	 * Get the depth image (does not make a new object, use get() if you need a copy)
	 * 
	 * @return reference to depth image 
	 */	
	public PImage getDepthImage() {
		return depthImage;
	}
	
	/**
	 * Get the video image (does not make a new object, use get() if you need a copy)
	 * 
	 * @return reference to video image 
	 */		
	public PImage getVideoImage() {
		return videoImage;
	}
}