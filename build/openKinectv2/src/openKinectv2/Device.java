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
		
		ptr = initJNI();
		
		depthImg = parent.createImage(512, 424, PImage.ALPHA);

		//System.out.println(version());
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
    public PImage getDepthImage(){
    	int[] depthMaskData = jniGetDepthData();
    	if(depthMaskData.length == depthImg.width * depthImg.height){
    		PApplet.arrayCopy(depthMaskData, 0, depthImg.pixels, 0, depthImg.width * depthImg.height);
    		depthImg.updatePixels();
    	}
		
		return depthImg;
    }
    
    //JNI Functions
    private  native long initJNI();
    private  native void openJNI();
    private  native void stopJNI();
    
    private  native String version();
    
    private  native int[] jniGetDepthData();
    
    
}