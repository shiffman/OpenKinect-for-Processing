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
	PShader shader;
	
    public Device(PApplet _p) {
		parent = _p;
		System.out.println("Starting Device");
		
		ptr = initJNI();
		
		depthImg = parent.createImage(512, 424, PImage.RGB);

		//System.out.println(version());
		
		shader = parent.loadShader(System.getProperty("user.dir")+"/data/color.glsl");
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
    	int[] depthMaskData = jniGetDepthData();
    	//if(depthMaskData.length == depthImg.width * depthImg.height){
    		PApplet.arrayCopy(depthMaskData, 0, depthImg.pixels, 0, depthImg.width * depthImg.height);
    		depthImg.updatePixels();
    		//System.out.println("get depth");
    	//}
		
		return depthImg;
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
    
    private  native int[] jniGetDepthData();
    
    
}