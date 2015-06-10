package openKinectv2;

import processing.core.PApplet;
import processing.core.PVector;

public class Device {
	
    static {
        System.loadLibrary("JNILibfreenect2");
    }
    
    public static void print() {
        System.out.println("java print");
    }
    
    private PApplet parent;
	private long ptr;
    
    public native String stringFromJNI();
    public native static void nativePrint();

    public static void main(String args[]) {
    	Device.nativePrint();
    	Device hellojni = new Device();
        System.out.println(hellojni.stringFromJNI());
    }
}