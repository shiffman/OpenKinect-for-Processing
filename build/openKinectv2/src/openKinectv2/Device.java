package openKinectv2;

import processing.core.PApplet;
import processing.core.PVector;

public class Device {
	
    static {
        System.loadLibrary("JNILibfreenect2");
    }
    
    
    private PApplet parent;
	private long ptr;

    
    public Device(PApplet _p) {
		parent = _p;
		System.out.println("Starting Device");
		
    }
    
    void openDevice(){
    	openJNI();
    	
    }
    
    void stopDevice(){
    	stopJNI();
    }
    
   
    
    //JNI Functions
    public native String stringFromJNI();
    public native static void nativePrint();
    public native static void openJNI();
    public native static void stopJNI();
    
    
}