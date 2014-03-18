//
//  KinectJNI.java
//  KinectJNI
//
//  Created by Daniel Shiffman on 11/14/10.
//  Copyright (c) 2010 ITP. All rights reserved.
//
//	For information on setting Java configuration information, including 
//	setting Java properties, refer to the documentation at
//		http://developer.apple.com/techpubs/java/java.html
//

package shiffman.kinect;

/**
 * @author daniel
 */
public class KinectJNI {
	public static void main(final String[] args) throws InterruptedException{
		System.out.println("FUCK YAH " + NativeKinect.getNum());
		
		/*NativeKinect.init();
		
		int counter = 0;
		while (true) {
			NativeKinect.update();
			int[] pixels = NativeKinect.getPixels();
			Thread.sleep(100);
			
			int color = pixels[pixels.length/2];
			System.out.println("COUNTER: " + counter + "  GOT PIXELS: " + pixels.length + "  A color: " + color);
  			counter++;
			if (counter > 300) {
				break;
			}
		}
		NativeKinect.close();*/
	}
}
