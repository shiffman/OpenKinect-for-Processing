package examples;

import openKinectv2.*;
import processing.core.*;


public class FrameTest extends PApplet{
	
	OpenKinectv2 openKinect2;

	public void setup(){
		size(512*3, 424, P3D);
		openKinect2 = new OpenKinectv2(this);
		openKinect2.open();
	}
	
	
	public void draw(){
		background(0);
		
		PImage depth = openKinect2.getDepthImage();
		image(depth, 0, 0);
		
		PImage ir = openKinect2.getIrImage();
		image(ir, 512, 0);
		
		PImage color = openKinect2.getColorImage();
		image(color, 512*2, 0, 512, 424);
		
		
		/*depth.loadPixels();
		for(int i = 0 ; i < 512; i+=1){
			for(int j = 0 ; j < 424; j+=1){
				int pixel = depth.pixels[512*j + i]+ 127;
				//println(pixel);
				//println(colorByte2Int(pixel));
				//stroke(colorByte2Int(pixel));
				stroke(pixel);
				point(i, j);
			}
		}
		*/
	}
	
	int  colorByte2Int(int gray){
		gray = gray & 0xffff;
	    return 0xff000000 | (gray << 16) | (gray << 8) | gray;
	}
	float valA = 0.04599f;
	float valR = 0.0722f;
	float valG = 0.7152f;
	float valB = 0.2126f;
	
	float min = 50;
	float max = 4500;
	

	float gamma = 1.0f;

	public void keyPressed(){
	if(key == '1'){
		openKinect2.changeA(valA);
		openKinect2.changeR(valR);
		openKinect2.changeG(valG);
		openKinect2.changeB(valB);
		
	}
	 if(key == 'a'){
		 valA += 0.001;
		 openKinect2.changeA(valA);
		 println("a "+valA);
	 }
	 
	 if(key == 's'){
		 valA -= 0.001;
		 openKinect2.changeA(valA);
		 println("a "+valA);
	 }
	 
	 if(key == 'z'){
		 valA += 0.1;
		 openKinect2.changeA(valA);
		 println("a "+valA);
	 }
	 
	 if(key == 'x'){
		 valA -= 0.1;
		 openKinect2.changeA(valA);
		 println("a "+valA);
	 }
	 
	 if(key == 'q'){
		 valA += 0.01;
		 openKinect2.changeA(valA);
		 println("a "+valA);
	 }
	 
	 if(key == 'w'){
		 valA -= 0.01;
		 openKinect2.changeA(valA);
		 println("a "+valA);
	 }
	 
	 
	 //R
	 if(key == 'e'){
		 valR += 0.001;
		 openKinect2.changeR(valR);
		 println("r "+valR);
	 }
	 
	 if(key == 'r'){
		 valR -= 0.001;
		 openKinect2.changeR(valR);
		 println(valR);
	 }
	 
	 if(key == 'd'){
		 valR += 0.1;
		 openKinect2.changeR(valR);
		 println("r "+valR);
	 }
	 
	 if(key == 'f'){
		 valR -= 0.1;
		 openKinect2.changeR(valR);
		 println("r "+valR);
	 }
	 
	 if(key == 'c'){
		 valR += 0.01;
		 openKinect2.changeR(valR);
		 println("r "+valR);
	 }
	 
	 if(key == 'v'){
		 valR -= 0.01;
		 openKinect2.changeR(valR);
		 println("r "+valR);
	 }
	 
	 //G
	 if(key == 't'){
		 valG += 0.001;
		 openKinect2.changeG(valG);
		 println("g "+valG);
	 }
	 
	 if(key == 'y'){
		 valG -= 0.001;
		 openKinect2.changeG(valG);
		 println("g "+valG);
	 }
	 
	 if(key == 'g'){
		 valG += 0.1;
		 openKinect2.changeG(valG);
		 println("g "+valG);
	 }
	 
	 if(key == 'h'){
		 valG -= 0.1;
		 openKinect2.changeG(valG);
		 println("g "+valG);
	 }
	 
	 if(key == 'b'){
		 valG += 0.01;
		 openKinect2.changeG(valG);
		 println("g "+valG);
	 }
	 
	 if(key == 'n'){
		 valG -= 0.01;
		 openKinect2.changeG(valG);
		 println("g "+valG);
	 }
	 
	 //b
	 if(key == 'u'){
		 valB += 0.001;
		 openKinect2.changeB(valB);
		 println("b "+valB);
	 }
	 
	 if(key == 'i'){
		 valB -= 0.001;
		 openKinect2.changeB(valB);
		 println("b "+valB);
	 }
	 
	 if(key == 'j'){
		 valB += 0.1;
		 openKinect2.changeB(valB);
		 println("b "+valB);
	 }
	 
	 if(key == 'k'){
		 valB -= 0.1;
		 openKinect2.changeB(valB);
		 println("b "+valB);
	 }
	 
	 if(key == 'm'){
		 valB += 0.01;
		 openKinect2.changeB(valB);
		 println("b "+valB);
	 }
	 
	 if(key == ','){
		 valB -= 0.01;
		 openKinect2.changeB(valB);
		 println("b "+valB);
	 }
	 
	 if(key == '2'){
		 openKinect2.mapDepth(true);
	 }
	 if(key == '3'){
		 openKinect2.mapDepth(false);
	 }
	 
	 if(key == '4'){
		 gamma += 0.1;
		 openKinect2.setGamma(gamma);
		 println("gamma "+gamma);
	 }
	 if(key == '5'){
		 gamma -= 0.1;
		 openKinect2.setGamma(gamma);
		 println("gamma "+gamma);
	 }
	 if(key == '6'){
		 gamma += 0.01;
		 openKinect2.setGamma(gamma);
		 println("gamma "+gamma);
	 }
	 if(key == '7'){
		 gamma -= 0.01;
		 openKinect2.setGamma(gamma);
		 println("gamma "+gamma);
	 }
	 
	 if(key == '8'){
		 openKinect2.changeR(0.0f);
		 println("r "+0);
	 }
	 
	 if(key == '9'){
		 openKinect2.changeG(0.0f);
		 println("g "+0);
	 }
	 
	 if(key == '0'){
		 openKinect2.changeB(0.0f);
		 println("b "+0);
	 }
	 if(key == '-'){
		 openKinect2.changeA(0.0f);
		 println("a "+0);
	 }
	 
	 
	 
	 if(key == 'o'){
		 min -=1;
		 openKinect2.setMin(min);
		 println("mi "+min);
	 }
	 
	 
	 if(key == 'p'){
		 min +=1;
		 openKinect2.setMin(min);
		 println("mi "+min);
	 }
	 
	 if(key == 'l'){
		 max -= 1;
		 openKinect2.setMax(max);
		 println("mx "+max);
	 }
	 if(key == ';'){
		 max +=50;
		 openKinect2.setMax(max);
		 println("mx "+max);
	 }
	 
	 
	 
	}
	
}
