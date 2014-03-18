package org.openkinect.processing;

import java.nio.ByteBuffer;
import java.nio.ShortBuffer;

import org.openkinect.Image;

import processing.core.PApplet;
import processing.core.PConstants;
import processing.core.PImage;

public class DepthImage  implements Image {
	long time;
	float fps;

	PApplet parent;
	public PImage img;

	boolean processImage = true;
	ShortBuffer sdata;
	
	public DepthImage(PApplet p) {
		parent = p;
		img = p.createImage(640,480,PConstants.RGB);
		time = System.currentTimeMillis();
	}
	
	public void enableImage(boolean b) {
		processImage = b;
	}
	
	public ShortBuffer getRawData() {
		return sdata;
	}
	
	public PImage getImage() {
		return img;
	}

	public void data(ByteBuffer data) {
		sdata = data.asShortBuffer();

		if (processImage) {
			for(int y=0; y<img.height; y++)
			{
				for(int x=0; x<img.width; x++)
				{
					int offset = x+y*img.width;
					short depth = sdata.get(offset);
					int d = Math.round((1 - (depth / 2047f)) * 255f);
					int pixel = (0xFF) << 24
					| (d & 0xFF) << 16
					| (d & 0xFF) << 8
					| (d & 0xFF) << 0;

					img.pixels[offset] = pixel;//pixel;
				}
			}

			img.updatePixels();
		}
		long now = System.currentTimeMillis();
		long passedTime = now-time;
		time = now;
		float currentFPS = 1.0f / (passedTime / 1000.0f);
		fps = PApplet.lerp(fps,currentFPS,0.1f);

	}


	static int depth2rgb(short depth)
	{
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

	static int depth2intensity(short depth)
	{
		int d = Math.round((1 - (depth / 2047f)) * 255f);
		int pixel = (0xFF) << 24
		| (d & 0xFF) << 16
		| (d & 0xFF) << 8
		| (d & 0xFF) << 0;

		return pixel;
	}

	public float getFPS() {
		return fps;
	}

}
