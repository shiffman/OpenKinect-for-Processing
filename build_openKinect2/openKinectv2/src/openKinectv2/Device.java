package openKinectv2;

import processing.core.PApplet;
import processing.core.PVector;
import processing.core.PImage;
import processing.opengl.PShader;


public class Device {
	
    static {
    	System.loadLibrary("turbojpeg");
        System.loadLibrary("JNILibfreenect2");
    }
    
    private PApplet parent;
	private long ptr;
	
	PImage depthImg;
	PImage irImg;
	PImage colorImg;
	PImage undistortedImg;
	PImage registeredImg;
	
	PShader shader;
	
	/**
	 * Constructor for openKinect 2
	 * @param _p PApplet parent
	 */
    public Device(PApplet _p) {
		parent = _p;
		System.out.println("Starting Device");
		
		ptr = initJNI();
		
		depthImg = parent.createImage(512, 424, PImage.ARGB);
		irImg 	 = parent.createImage(512, 424, PImage.ALPHA);
		colorImg = parent.createImage(1920, 1080, PImage.ARGB);
		undistortedImg = parent.createImage(512, 424, PImage.ARGB);
		registeredImg  = parent.createImage(512, 424, PImage.ARGB);
		
		//System.out.println(version());
    }
    
   
    /**
     * Open and initialize the Device
     */
    public void openDevice(){
    	openJNI();
    	
    }
    
    /**
     * Close Device
     */
    public void stopDevice(){
    	stopJNI();
    }
    
    public void changeA(float value){
    	jniChangeA(value);
    }
    public void changeR(float value){
    	jniChangeR(value);
    }
    public void changeG(float value){
    	jniChangeG(value);
    }
    public void changeB(float value){
    	jniChangeB(value);
    }
    
    public void mapDepth(boolean val){
    	jniDepth(val);
    }
    
    public void setGamma(float value){
    	jniGamma(value);
    }
    
    public void setMin(float v){
    	jniMin(v);
    }
    
    public void setMax(float v){
    	jniMax(v);
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
    	PApplet.arrayCopy(registeredData, 0, registeredImg.pixels, 0, registeredImg.width* registeredImg.height);;
    	registeredImg.updatePixels();
    	return registeredImg;
    }
    

    
    //JNI Functions
    private  native long initJNI();
    private  native void openJNI();
    private  native void stopJNI();
    
    private native void jniChangeA(float value);
    private native void jniChangeR(float value);
    private native void jniChangeG(float value);
    private native void jniChangeB(float value);
    private native void jniDepth(boolean val);
    
    private native void jniMin(float val);
    private native void jniMax(float val);
    
    private native void jniGamma(float val);
    private  native String version();
    
    private  native int [] jniGetDepthData();
    private  native int [] jniGetIrData();
    private  native int [] jniGetColorData();
    private  native int [] jniGetUndistorted();
    private  native int [] jniGetRegistered();
}