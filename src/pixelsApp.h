#pragma once

#include "ofMain.h"
#include "ofAppEGLWindow.h"
#include "VideoSource.h"
//#include "ofxRPiCameraVideoGrabber.h"
#include "ofxOpenCv.h"


class pixelsApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();
		void mousePressed(int x, int y, int button);

//	ofxRPiCameraVideoGrabber videoGrabber;
//	OMXCameraSettings omxCameraSettings;
	
	PiCameraSource videoGrabber;
	
	ofxCvColorImage rgb,hsb;
	ofxCvGrayscaleImage hue,sat,bri,filtered;
	ofxCvContourFinder contours;
	
	int w, h;
	int findHue;
};

