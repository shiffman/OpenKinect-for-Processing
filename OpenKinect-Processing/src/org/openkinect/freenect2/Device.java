package org.openkinect.freenect2;

import processing.core.PApplet;
import processing.core.PVector;
import processing.core.PImage;
import processing.opengl.PShader;

/*
openKinect2 library for Processing
Copyright (c) 2014 Thomas Sanchez Lengeling

* Redistribution and use in source and binary forms, with or
* without modification, are permitted provided that the following
* conditions are met:
*
* Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in
* the documentation and/or other materials provided with the
* distribution.
*

openKinect2 library for Processing is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

openKinect2 for Processing is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with openKinect2  for Processing.  If not, see
<http://www.gnu.org/licenses/>.
*/

public class Device {
	
    static {
    	System.loadLibrary("turbojpeg");
        System.loadLibrary("JNILibfreenect2");
    }
    
    public int depthWidth = 512;
    public int depthHeight = 424;
    
    public int colorWidth = 1920;
    public int colorHeight = 1080;
    
    private PApplet parent;
	private long ptr;
	
	//version control for openKinect2
	private String version = "";
		
	PImage depthImg;
	PImage irImg;
	PImage colorImg;
	PImage undistortedImg;
	PImage registeredImg;
	
	/**
	 * Constructor for openKinect 2
	 * @param _p PApplet parent
	 */
    public Device(PApplet _p) {
		parent = _p;
		
		ptr = jniInit();
		
		depthImg = parent.createImage(depthWidth, depthHeight, PImage.ALPHA);
		irImg 	 = parent.createImage(depthWidth, depthHeight, PImage.ALPHA);
		colorImg = parent.createImage(colorWidth, colorHeight, PImage.ARGB);
		undistortedImg = parent.createImage(depthWidth, depthHeight, PImage.ALPHA);
		registeredImg  = parent.createImage(depthWidth, depthHeight, PImage.ARGB);
		
		depthImg.loadPixels();
		irImg.loadPixels();
		colorImg.loadPixels();
		undistortedImg.loadPixels();
		registeredImg.loadPixels();
		
		for(int i = 0; i < depthWidth; i++){
			for(int j = 0; j < depthHeight; j++){
				int pos = i + j * depthWidth;
				depthImg.pixels[pos] = parent.color(0);
				irImg.pixels[pos] = parent.color(0);
				undistortedImg.pixels[pos] = parent.color(0);
				registeredImg.pixels[pos] = parent.color(0);
			}
		}
		
		depthImg.updatePixels();
		irImg.updatePixels();
		undistortedImg.updatePixels();
		registeredImg.updatePixels();

		
		for(int i = 0; i < colorWidth; i++){
			for(int j = 0; j < colorHeight; j++){
				int pos = i + j * colorWidth;
				colorImg.pixels[pos] = parent.color(0);
			}
		}
		
		colorImg.updatePixels();

		
		//System.out.println(version());
    }
    
   
    /**
     * Open and initialize the a default Device
     */
    public void start(){
    	jniOpen();
    	
    }
    
    /**
     * Open and initialize a specific Device index [0, numDevices - 1]
     * @param index
     */
    public void start(int index){
    	jniOpenM(index);
    }
    
    
    /**
     * Close Device
     */
    public void stopDevice(){
    	jniStop();
    }    
    
    /**
     * Process Depth Image (512 x 424)
     * @return PImage
     */
    public PImage getDepthImage(){
    	int[] depthRawData = jniGetDepthData();
    	PApplet.arrayCopy(depthRawData, 0, depthImg.pixels, 0, depthImg.width * depthImg.height);
    	depthImg.updatePixels();
		return depthImg;
    }
    
    /**
     * Process Infrared Image (512 x 424)
     * @return PImage
     */
    public PImage getIrImage(){
    	int[] irRawData = jniGetIrData();
    	PApplet.arrayCopy(irRawData, 0, irImg.pixels, 0, irImg.width * irImg.height);
    	irImg.updatePixels();
		return irImg;
    }
    
    
    /**
     *  Process Color Image (1920 x 1080)
     * @return PImage
     */
    public PImage getVideoImage(){
    	int[] colorRawData = jniGetColorData();
    	PApplet.arrayCopy(colorRawData, 0, colorImg.pixels, 0, colorImg.width * colorImg.height);
    	colorImg.updatePixels();
		return colorImg;
    }
    
    /**
     * Process the undistored Image  (512 x 442) Image output
     * @return PImage 
     */
    public PImage getUndistoredImage(){
    	int[] undistoredData = jniGetUndistorted();
    	PApplet.arrayCopy(undistoredData, 0, undistortedImg.pixels, 0, undistortedImg.width* undistortedImg.height);;
    	undistortedImg.updatePixels();
    	return undistortedImg;
    }
    
    /**
     * Process the registered Image RGB and Depth mapping (512 x 442) Image output
     * @return PImage
     */
    public PImage getRegisteredImage(){
    	int[] registeredData = jniGetRegistered();
    	PApplet.arrayCopy(registeredData, 0, registeredImg.pixels, 0, registeredImg.width* registeredImg.height);
    	registeredImg.updatePixels();
    	return registeredImg;
    }
    
    /**
     * get the raw depth data 512 x 424
     * @return array of ints from 0 - 45000
     */
    public int []  getRawDepth(){
    	return jniGetRawDepthData();
    }
    
    /**
     * Get the number of Devices connected to the computer
     * @return int
     */
    public int  getNumKinects(){
    	return jniGetNumDevices();
    }
    
    /**
     * Get the Serial Number 
     * @return
     */
    public String getDefaulSerialNum(){
    	if(jniGetNumDevices() > 0)
    		return jniGetSerialDevice(0);
    	return "123456789";
    }
    
    
    /**
     * Get the Serial Number 
     * @return
     */
    public String getSerialNum(int index){
    	if(jniGetNumDevices() > 0 && jniGetNumDevices() < index)
    		return jniGetSerialDevice(index);
    	return "123456789";
    }
    
    /**
     * Print Number of Kinect v2 connected and Information about the Devices
     */
    public void printDevices(){
    	jniEumerateDevices();
    }
    
    /**
     * Enable the depth capture of the Kinect 2
     */
    public void startDepth(){
    	jniEnableDepth(true);
    }
    
    /**
     * Enable the Video capture of the Kinect 2
     */
    public void startVideo(){
    	jniEnableVideo(true);
    }
    
    /**
     * Enable the IR capture of the Kinect 2
     */
    public void startIR(){
    	jniEnableIR(true);
    }
    
    /**
     * Enable the Video + Depth mapping of the Kinect 2
     */
    public void startRegistered(){
    	jniEnableRegistered(true);
    }
    
    
    
    
    public void toggleDepth(){
    	
    }
    
    public void toggleVideo(){
    	
    }
    
    public void toggleIR(){
    	
    }
    
    public void toggleRegistered(){
    	
    }
    
    
    //JNI Functions
    private  native long 	jniInit();
    private  native void 	jniOpen();
    private  native void 	jniOpenM(int index);
    private  native void 	jniStop();
    private  native void 	jniEumerateDevices();
    
    //Multiple Kinect Funtions
    private  native void    jniOpenSerial(String serialNumber);
    private  native int	    jniGetNumDevices();
    private  native String  jniGetSerialDevice(int index);
    
    //Get data functions
    private  native int []  jniGetDepthData();
    private  native int []  jniGetRawDepthData();
    
    private  native int []  jniGetIrData();
    private  native int []  jniGetColorData();
    private  native int []  jniGetUndistorted();
    private  native int []  jniGetRegistered();
    
    //Enables functions
    private  native void jniEnableVideo(boolean enable);
    private  native void jniEnableDepth(boolean enable);
    private  native void jniEnableIR(boolean enable);
    private  native void jniEnableRegistered(boolean enable);
        
    //toggle Function
    private  native void jniToggleVideo(boolean toggle);
    private  native void jniToggleDepth(boolean toggle);
    private  native void jniToggleIR(boolean toggle);
    private  native void jniToggleRegistered(boolean toggle);

}