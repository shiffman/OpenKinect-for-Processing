package openKinectv2;

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

openKinect2for Processing is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with KinectfV2.0 library for Processing.  If not, see
<http://www.gnu.org/licenses/>.
*/

public class Device {
	
    static {
    	System.loadLibrary("turbojpeg");
        System.loadLibrary("JNILibfreenect2");
    }
    
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
		
		depthImg = parent.createImage(512, 424, PImage.ALPHA);
		irImg 	 = parent.createImage(512, 424, PImage.ALPHA);
		colorImg = parent.createImage(1920, 1080, PImage.ARGB);
		undistortedImg = parent.createImage(512, 424, PImage.ALPHA);
		registeredImg  = parent.createImage(512, 424, PImage.ARGB);
		
		for(int i = 0; i < 512; i++){
			for(int j = 0; j < 424; j++){
				depthImg.set(i, j, parent.color(0));
				irImg.set(i, j, parent.color(0));
				undistortedImg.set(i, j, parent.color(0));
				registeredImg.set(i, j, parent.color(0));
			}
		}
		
		for(int i = 0; i < 1920; i++){
			for(int j = 0; j < 1080; j++){
				colorImg.set(i, j, parent.color(0));
			}
		}
		
		//System.out.println(version());
    }
    
   
    /**
     * Open and initialize the Device
     */
    public void open(){
    	jniOpen();
    	
    }
    
    public void open(int index){
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
    public PImage getColorImage(){
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
    public int []  getRawDepthData(){
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
     * Print Number of Kinect v2 connected 
     */
    public void printDevices(){
    	jniEumerateDevices();
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
    

    private  native int []  jniGetDepthData();
    private  native int []  jniGetRawDepthData();
    
    private  native int []  jniGetIrData();
    private  native int []  jniGetColorData();
    private  native int []  jniGetUndistorted();
    private  native int []  jniGetRegistered();
}