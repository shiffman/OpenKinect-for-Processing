package org.openkinect.processing;

import org.openkinect.freenect2.Device;

import processing.core.PApplet;

/*
openKinect2 library for Processing
Copyright (c) 2014 Thomas Sanchez Lengeling

* Redistribution and use in source and binary forms, with or
* without modification, are permitted provided that the following
* conditions are met:
*
* Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in
* the documentation and/or other materials provided with the
* distribution.
*

openKinect2 library for Processing is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

openKinect2 for Processing is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with openKinect2 library for Processing.  If not, see
<http://www.gnu.org/licenses/>.
*/

public class Kinect2 extends Device{
	
	public static PApplet parent;
	
	public Kinect2(PApplet _p) {
		super(_p);	
		parent = _p;
		
		parent.registerMethod("dispose", this);
	}


	public void dispose() {
		System.out.println("EXIT");
		stopDevice();
	}
	
	
}