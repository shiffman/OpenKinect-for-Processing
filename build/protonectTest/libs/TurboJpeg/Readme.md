Rough OF addon that uses turbo-jpeg lib to read and write jpegs. 2-4 times faster than OF's freeImge based jpeg en/decoder.

You will need to install libjpeg-turbo (http://sourceforge.net/projects/libjpeg-turbo/) and its dylibs or freemImage's internal version of libjpeg will conflict with the one required by libjpeg-turbo, throwing a "Wrong JPEG library version: library is 80, caller expects 62" error. Alternatively you can just place the "libturbojpeg.dylib" by the binary and it should work. Only tested on OSX Lion.

	ofxTurboJpeg turbo;
	
	//load jpeg
	ofImage * image = turbo.load("loadTest.jpg");
	
	//save jpeg
	turbo.save(image, "save turbo.jpg", 75);

![ofxTurboJpeg screenshot](http://farm8.staticflickr.com/7243/6999702551_fc8812d210_z.jpg)

