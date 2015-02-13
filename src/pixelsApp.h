#pragma once

#include "ofMain.h"
#include "ofAppEGLWindow.h"
#include "VideoSource.h"
//#include "ofxRPiCameraVideoGrabber.h"
#include "ofxOpenCv.h"

class color
{
public:
	
	float hue, sat;
	
	ofxVec2f pos;
};


class pixelsApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();
		void mousePressed(int x, int y, int button);

	PiCameraSource videoGrabber;
	
	ofxCvColorImage colorImg; //Main color image which is gonna get wrapped
	ofxCvColorImage colorImgHSV; //the image doing the wrapping
	
	ofxCvGrayscaleImage hueImg; //Hue map
	ofxCvGrayscaleImage satImg; //Saturation map
	ofxCvGrayscaleImage briImg; //Brightness map
	
	ofxCvGrayscaleImage reds; //Grayscale image we are gonna run the contour finder over to find our color
	
	color one; //color that we're gonna track
	
	unsigned char * colorTrackedPixelsRed; //just some raw images which we are gonna put pixels into
	ofTexture trackedTextureRed; //color texture that we are gonna draw to
	
	ofxCvContourFinder finderRed; //contour finder, very handy
	int						camWidth, camHeight;
};

