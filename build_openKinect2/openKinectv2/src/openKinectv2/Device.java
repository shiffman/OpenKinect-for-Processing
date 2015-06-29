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
		
		//shader = parent.loadShader(System.getProperty("user.dir")+"/data/color.glsl");
    }
    
    void openDevice(){
    	openJNI();
    	
    }
    
    void stopDevice(){
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
     * Process Depth Image
     * @return
     */
    public PImage getDepthImage(){
    	int[] depthRawData = jniGetDepthData();
    	//if(depthMaskData.length == depthImg.width * depthImg.height){
    		PApplet.arrayCopy(depthRawData, 0, depthImg.pixels, 0, depthImg.width * depthImg.height);
    		depthImg.updatePixels();
    		//System.out.println("get depth");
    	//}
		
		return depthImg;
    }
    
    /**
     * Process Infrared Image
     * @return
     */
    public PImage getIrImage(){
    	int[] irRawData = jniGetIrData();
    	//if(depthMaskData.length == depthImg.width * depthImg.height){
    		PApplet.arrayCopy(irRawData, 0, irImg.pixels, 0, irImg.width * irImg.height);
    		irImg.updatePixels();
    		//System.out.println("get depth");
    	//}
		
		return irImg;
    }
    
    
    /**
     *  Process Color Image
     * @return
     */
    public PImage getColorImage(){
    	int[] colorRawData = jniGetColorData();
    	//if(depthMaskData.length == depthImg.width * depthImg.height){
    		PApplet.arrayCopy(colorRawData, 0, colorImg.pixels, 0, colorImg.width * colorImg.height);
    		colorImg.updatePixels();
    		//System.out.println("get depth");
    	//}
		
		return colorImg;
    }
    
    public PImage getUndistoredImage(){
    	int[] undistoredData = jniGetUndistorted();
    	PApplet.arrayCopy(undistoredData, 0, undistortedImg.pixels, 0, undistortedImg.width* undistortedImg.height);;
    	undistortedImg.updatePixels();
    	return undistortedImg;
    }
    
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