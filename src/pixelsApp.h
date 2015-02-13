#pragma once

#include "ofMain.h"
#include "ofAppEGLWindow.h"
#include "VideoSource.h"
#include "ofxOpenCv.h"

class pixelsApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void mousePressed(int x, int y, int button);

	PiCameraSource videoGrabber;
	ofxCvColorImage rgb,hsb;
	ofxCvGrayscaleImage hue,sat,bri,filtered;
	ofxCvContourFinder contours;

	int w,h;
	int findHue, findSat, findBri, anchoHue, anchoSat, anchoBri;

	bool verHsb, verSat, verHue, verInput;
};
