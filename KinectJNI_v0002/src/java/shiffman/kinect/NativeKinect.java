//
//  NativeAddressBook.java
//  KinectJNI
//
//  Created by Daniel Shiffman on 11/14/10.
//  Copyright (c) 2010 ITP. All rights reserved.
//

package shiffman.kinect;

import java.util.*;

/**
 * Abstraction for the Mac OS X AddressBook API
 * 
 * Creates a copy of contacts from the address book, and vends Person
 * objects. Uses JNI to obtain simple Lists, Maps, Strings, and Number
 * representations of
 * 
 * @author daniel
 */
public class NativeKinect {
	static {
		// ensure native JNI library is loaded
		System.loadLibrary("Kinect");
	}
	
	// JNI method which obtains the UID of the "me" card in Address Book.app
	private static native int getNumber();
	
	public static int getNum() {
		return getNumber();
	}

}
