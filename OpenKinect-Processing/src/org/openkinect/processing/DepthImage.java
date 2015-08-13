package org.openkinect.processing;

import java.nio.ShortBuffer;
import processing.core.PImage;
import processing.core.PVector;

class DepthImage {

	//Camera params
	static double fx_d = 1.0 / 5.9421434211923247e+02;
	static double fy_d = 1.0 / 5.9104053696870778e+02;
	static double cx_d = 3.3930780975300314e+02;
	static double cy_d = 2.4273913761751615e+02;
	
	static void data(ShortBuffer data, PImage img, int [] rawDepth, float [] depthLookUp, float [] depthToWorld, boolean colorDepth, boolean mirrorMode) {
		
		img.loadPixels();
		if(mirrorMode){ //mirror mode enable
			if(colorDepth){
				for(int y = 0; y < img.height; y++) {
					for(int x = 0; x < img.width; x++) {
						int offsetMirror = img.width - x - 1 + y * img.width;
						int offset = x + y*img.width;
						short depth = data.get(offsetMirror);
						img.pixels[offset] = depth2rgb(depth);
						rawDepth[offset] = depth;
						
						//depth to world 
						PVector depthWorld = depthToWorld(x, y, depthLookUp, depth);
						depthToWorld[offset*3] =  depthWorld.x;
						depthToWorld[offset*3 + 1] =  depthWorld.y;
						depthToWorld[offset*3 + 2] =  depthWorld.z;
					}
				}
			}else{
				for(int y = 0; y < img.height; y++) {
					for(int x = 0; x < img.width; x++) {
						int offsetMirror = img.width - x - 1 + y * img.width;
						int offset = x + y*img.width;
						short depth = data.get(offsetMirror);
						img.pixels[offset] = depth2intensity(depth);//(int) (depth == 0 || depth > 2047 ? 0 : 255 - (((float)depth / 2047.0f) * 255.0f));}
						rawDepth[offset] = depth;
						
						//depth to world 
						PVector depthWorld = depthToWorld(x, y, depthLookUp, depth);
						depthToWorld[offset*3] =  depthWorld.x;
						depthToWorld[offset*3 + 1] =  depthWorld.y;
						depthToWorld[offset*3 + 2] =  depthWorld.z;
					}
				}
			}
		}else{ //no mirror mode
			if(colorDepth){
				for(int y = 0; y < img.height; y++) {
					for(int x = 0; x < img.width; x++) {
						int offset = x + y*img.width;
						short depth = data.get(offset);
						img.pixels[offset] = depth2rgb(depth);
						rawDepth[offset] = depth;
						
						//depth to world 
						PVector depthWorld = depthToWorld(x, y, depthLookUp, depth);
						depthToWorld[offset*3] =  depthWorld.x;
						depthToWorld[offset*3 + 1] =  depthWorld.y;
						depthToWorld[offset*3 + 2] =  depthWorld.z;
					}
				}
			}else{
				for(int y = 0; y < img.height; y++) {
					for(int x = 0; x < img.width; x++) {
						int offset = x + y*img.width;
						short depth = data.get(offset);
						img.pixels[offset] = depth2intensity(depth);//(int) (depth == 0 || depth > 2047 ? 0 : 255 - (((float)depth / 2047.0f) * 255.0f));}
						rawDepth[offset] = depth;
						
						//depth to world 
						PVector depthWorld = depthToWorld(x, y, depthLookUp, depth);
						depthToWorld[offset*3] =  depthWorld.x;
						depthToWorld[offset*3 + 1] =  depthWorld.y;
						depthToWorld[offset*3 + 2] =  depthWorld.z;
					}
				}
			}
		}

		img.updatePixels();
	}
	
	

	static int depth2rgb(short depth) {
		int r,g,b;

		float v = depth / 2047f;
		v = (float) Math.pow(v, 3)* 6;
		v = v*6*256;

		int pval = Math.round(v);
		int lb = pval & 0xff;
		switch (pval>>8) {
		case 0:
			b = 255;
			g = 255-lb;
			r = 255-lb;
			break;
		case 1:
			b = 255;
			g = lb;
			r = 0;
			break;
		case 2:
			b = 255-lb;
			g = 255;
			r = 0;
			break;
		case 3:
			b = 0;
			g = 255;
			r = lb;
			break;
		case 4:
			b = 0;
			g = 255-lb;
			r = 255;
			break;
		case 5:
			b = 0;
			g = 0;
			r = 255-lb;
			break;
		default:
			r = 0;
			g = 0;
			b = 0;
			break;
		}

		int pixel = (0xFF) << 24
				| (b & 0xFF) << 16
				| (g & 0xFF) << 8
				| (r & 0xFF) << 0;

		return pixel;
	}

	static int depth2intensity(short depth) {
		int d = Math.round((1 - (depth / 2047f)) * 255f);
		int pixel = (0xFF) << 24
				| (d & 0xFF) << 16
				| (d & 0xFF) << 8
				| (d & 0xFF) << 0;

		return pixel;
	}
	
	static PVector depthToWorld(int x, int y, float [] depthLookUp, int depthValue) {
		PVector result = new PVector();
		double depth =  depthLookUp[depthValue];
		result.x = (float)((x - cx_d) * depth * fx_d);
		result.y = (float)((y - cy_d) * depth * fy_d);
		result.z = (float)(depth);
		return result;
	}

}
