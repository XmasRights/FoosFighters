 #include "pixelsApp.h"

//Pixel access is not implemented in the player
//so here is how to do it yourself

//it should be implemented along with an "isFrameNew" but that isn't working yet

//--------------------------------------------------------------
void pixelsApp::setup()
{
	camWidth = 320; // try to grab at this size.
	camHeight = 240;
	one.pos = ofxVec2f(0,0);

	videoGrabber.setup(w, h);
	
	colorImg.allocate(camWidth,camHeight); //Image that will house the cameras output, used because of laziness
	
	colorImgHSV.allocate(camWidth,camHeight); //our HSB image that will house the color image and deal out the Hue, Saturation and brightness
	
	hueImg.allocate(camWidth,camHeight); //Hue map
	satImg.allocate(camWidth,camHeight); //saturation map
	briImg.allocate(camWidth,camHeight); //brightness map, not gonna be used but necessary
	
	reds.allocate(camWidth, camHeight); //our postRange image basically
	
	colorTrackedPixelsRed = new unsigned char [camWidth * camHeight]; //rangeImage
	
	trackedTextureRed.allocate(camWidth, camHeight, GL_LUMINANCE); //final output
	
	doDrawInfo = false;
}	

//--------------------------------------------------------------
void pixelsApp::update()
{
	if (videoGrabber.isFrameNew())
	{
		colorImgHSV = colorImg; //now we stuff the colorImg into our HSB image
		colorImgHSV.convertRgbToHsv(); //now we convert the colorImg inside colorImgHSV into HSV
		
		colorImgHSV.convertToGrayscalePlanarImages(hueImg, satImg, briImg); //distribute the hue, saturation and brightness to hueImg, satImg, and briImg
		
		// As stated in the following discussion, due to a probable bug in ofxOpenCv addon,
		// you have to explicitly call flagImageChanged() after convertToGrayscalePlanarImages().
		// http://forum.openframeworks.cc/t/converttograyscaleplanarimages-problem/2693/0
		hueImg.flagImageChanged();
		satImg.flagImageChanged();
		briImg.flagImageChanged();
		
		// ok cool, here we go:
		unsigned char * huePixels = hueImg.getPixels(); //huePixels is now a raw array of pixels
		unsigned char * satPixels = satImg.getPixels(); //satPixels is now a raw array of pixels just like huePixels
		//unsigned char * briPixels = briImg.getPixels();
		int nPixels = camWidth * camHeight; //get the number of pixels in the images since these raw images are continuous, so no breaks
		//so pixel number camWidth + 1 would be the first pixels in the second row of pixels of the image
		
		/* huePixels is a gigantic black and white array. so every pixel has a value from 0 to 255. This represents the hue values from the original color image. Certain colors can
		 be represented by certain hue ranges. hues from 4 to 21 are redish while 109 to 115 are green*/
		
		for (int i = 0; i < nPixels; i++){ //let's go through every pixel in hue pixels
			if ((huePixels[i] >= one.hue - 12 && huePixels[i] <= one.hue + 12) && //if the hue is of a certain range
				(satPixels[i] >= one.sat - 24 && satPixels[i] <= one.sat + 200)){ //if the saturation is of a certain range
				colorTrackedPixelsRed[i] = 255; //mark this corresponding pixel white
			} else {
				colorTrackedPixelsRed[i] = 0; //if it doesn't fit then color the corresponding pixel black
			}
		}
		
		reds.setFromPixels(colorTrackedPixelsRed, camWidth, camHeight); //set reds from the colorTrackedPixelsRed array so it's all clean and openCv operable
		finderRed.findContours(reds, 10,nPixels/3, 1, false, true); //lets find one (1) blob in the grayscale openCv image reds
		
		trackedTextureRed.loadData(colorTrackedPixelsRed, camWidth, camHeight, GL_LUMINANCE); //load up the data from the colorTrackedPixelsRed into a texture
		
		//------------------------------
		
		if(finderRed.blobs.size() > 0) {
			one.pos = ofxVec2f(finderRed.blobs[0].centroid.x,finderRed.blobs[0].centroid.y); //if the blob exists, set it's associated color (one) to it's position
		}
	}
}


//--------------------------------------------------------------
void pixelsApp::draw()
{
	ofSetColor(255,255,255);
	
	if (videoGrabber.isFrameNew())
	{
		colorImg.setFromPixels(videoGrabber.getPixels());
	}
	
	ofSetColor(0xffffff); //set a white color as the setColor
	vidGrabber.draw(0,0); //draw our video for reference/viewing pleasure
	colorImgHSV.draw(340, 0);
	
	trackedTextureRed.draw(20, 300); //draw everything that was found
	ofDrawBitmapString("red",20, 280); //label
	finderRed.draw(); //draw our contour tracker over the video
	
	glPushMatrix(); //start a new openGL stack
	glTranslatef(20,300,0); //translate lower a bit
	finderRed.draw(); //draw the contour tracker over the trackedTextureRed
	glPopMatrix(); //end the stack
	
	if(finderRed.blobs.size() > 0) { //if the blob exists then state it's x and y
		char tempStr1[255];
		sprintf(tempStr1, "x : %f\ny : %f", finderRed.blobs[0].centroid.x, finderRed.blobs[0].centroid.y);
		ofDrawBitmapString(tempStr1, 20, 250); //draw the string
	}
}


void testApp::mousePressed(int x, int y, int button){
	
	unsigned char * huePixels = hueImg.getPixels(); //teh hue
	unsigned char * satPixels = satImg.getPixels(); //teh saturation
	/*unsigned char * briPixels = briImg.getPixels(); //teh brightness*/ //unnecessary really, hue and sat should be enough
	
	x = MIN(x,hueImg.width-1); //find the smallest value out of those two so we don't crash if we click outside of the camera image
	y = MIN(y,hueImg.height-1);
	
	if(button == 0) {
		one.hue = huePixels[x+(y*hueImg.width)]; //set the hue
		one.sat = satPixels[x+(y*satImg.width)]; //set the sat
		/*one.bri = briPixels[x+(y*briImg.width)];*/
	}
}

