package openKinectv2;

public class Device {
	
    static {
        System.loadLibrary("JNILibfreenect2");
    }
    
    public static void print() {
        System.out.println("java print");
    }
    
    public native String stringFromJNI();
    public native static void nativePrint();

    public static void main(String args[]) {
    	Device.nativePrint();
    	Device hellojni = new Device();
        System.out.println(hellojni.stringFromJNI());
    }
}