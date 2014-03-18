package org.openkinect.processing;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.ShortBuffer;


import processing.core.PApplet;
import processing.core.PImage;

public class RGBImage extends DepthImage {
	boolean IR = false;
	

	public RGBImage(PApplet p) {
		super(p);
	}

	public void setIR(boolean b) {
		IR = b;
	}

	public synchronized void data(ByteBuffer data) {
		if (IR) {
			ShortBuffer sdata = data.asShortBuffer();
			for(int y=0; y<img.height; y++) {
				for(int x=0; x<img.width; x++) {
					int offset = x+y*img.width;
					int depth = sdata.get(offset);
					int d = PApplet.constrain(depth,0,255);//Math.round((1 - (depth / 2047f)) * 255f);
					int pixel = (0xFF) << 24
					| (d & 0xFF) << 16
					| (d & 0xFF) << 8
					| (d & 0xFF) << 0;
					img.pixels[offset] = pixel;
				}
			}
		} else {
			for(int y=0; y<img.height; y++) {
				for(int x=0; x<img.width; x++) {
					int offset = 3*(y*img.width+x);

					int r = data.get( offset+2 ) & 0xFF;
					int g = data.get( offset+1 ) & 0xFF;
					int b = data.get( offset+0 ) & 0xFF;

					int pixel = (0xFF) << 24
					| (b & 0xFF) << 16
					| (g & 0xFF) << 8
					| (r & 0xFF) << 0;
					img.pixels[x+img.width*y] = pixel;
				}
			}
		}

		
		img.updatePixels();

		long now = System.currentTimeMillis();
		long passedTime = now-time;
		time = now;


		float currentFPS = 1.0f / (passedTime / 1000.0f);
		fps = PApplet.lerp(fps,currentFPS,0.1f);
	}

}
