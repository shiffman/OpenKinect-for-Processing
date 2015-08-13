package org.openkinect.processing;

import java.nio.ByteBuffer;
import processing.core.PImage;

class RGBImage {

	static void data(ByteBuffer data, PImage img, boolean irMode, boolean mirrorMode) {
		if(mirrorMode){ //mirror enable
			if (irMode) {
				for(int y=0; y<img.height; y++) {
					for(int x=0; x<img.width; x++) {
						int offsetMirror = img.width - x - 1 + y * img.width;
						int offset = (y*img.width+x);
						
						int d = data.get(offsetMirror);
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
						
						int offsetMirror = (img.width - x - 1 + y * img.width)*3;
						int offset = (y*img.width+x);
		
						int r = data.get( offsetMirror + 2 ) & 0xFF;
						int g = data.get( offsetMirror + 1 ) & 0xFF;
						int b = data.get( offsetMirror) & 0xFF;
		
						int pixel = (0xFF) << 24
								| (b & 0xFF) << 16
								| (g & 0xFF) << 8
								| (r & 0xFF) << 0;
						img.pixels[offset] = pixel;
					}
				}
			}
		}else{ //normal mode, no mirror
			if (irMode) {
				for(int y=0; y<img.height; y++) {
					for(int x=0; x<img.width; x++) {
						int offset = x+y*img.width;
						int d = data.get(offset);
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
			
		}
		img.updatePixels();

	}
}
