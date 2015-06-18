package openKinectv2;

import processing.core.PApplet;
import processing.core.PVector;
import processing.core.PImage;

public class Device {
	
    static {
    	System.loadLibrary("turbojpeg");
        System.loadLibrary("JNILibfreenect2");
    }
    
    
    private PApplet parent;
	private long ptr;
	
	PImage depthImg;

    
    public Device(PApplet _p) {
		parent = _p;
		System.out.println("Starting Device");
		
		depthImg = parent.createImage(512, 424, PImage.ALPHA);
    }
    
    void openDevice(){
    	openJNI();
    	
    }
    
    void stopDevice(){
    	stopJNI();
    }
    
    /**
     * Process Depth Image
     * @return
     */
    PImage getDepthImage(){
    	int[] depthMaskData = jniGetDepthData();
    	PApplet.arrayCopy(depthMaskData, 0, depthImg.pixels, 0, depthImg.width * depthImg.height);
    	depthImg.updatePixels();
		
		return depthImg;
    }
    
    
    
   
    
    //JNI Functions
    public native static void openJNI();
    public native static void stopJNI();
    
    public native int[] jniGetDepthData();
    
    
}