 #include "pixelsApp.h"

//Pixel access is not implemented in the player
//so here is how to do it yourself

//it should be implemented along with an "isFrameNew" but that isn't working yet

//--------------------------------------------------------------
void pixelsApp::setup()
{
	w = 320;
	h = 240;
	
	videoGrabber.setup(w, h);

	rgb.allocate(w, h);
	hsb.allocate(w, h);
	hue.allocate(w, h);
	sat.allocate(w, h);
	bri.allocate(w, h);
	filtered.allocate(w, h);
}

//--------------------------------------------------------------
void pixelsApp::update()
{
	if (videoGrabber.isFrameNew())
	{
		//mirror horizontal
		rgb.mirror(false, true);

		//duplicate rgb
		hsb = rgb;

		//convert to hsb
		hsb.convertRgbToHsv();

		//store the three channels as grayscale images
		hsb.convertToGrayscalePlanarImages(hue, sat, bri);

		//filter image based on the hue value were looking for
		for (int i=0; i<w*h; i++) {
			filtered.getPixels()[i] = ofInRange(hue.getPixels()[i],findHue-5,findHue+5) ? 255 : 0;
		}
		filtered.flagImageChanged();

		//run the contour finder on the filtered image to find blobs with a certain hue
		contours.findContours(filtered, 50, w*h/2, 1, false);
	}
}


//--------------------------------------------------------------
void pixelsApp::draw()
{
	ofSetColor(255,255,255);

	if (videoGrabber.isFrameNew())
	{
		rgb.setFromPixels(videoGrabber.getPixels());
	}

	//draw all cv images
	rgb.draw(0,0);
	hsb.draw(640,0);
	hue.draw(0,240);
	sat.draw(320,240);
	bri.draw(640,240);
	filtered.draw(0,480);
	contours.draw(0,480);

	ofSetColor(255, 0, 0);
	ofFill();

	//draw red circles for found blobs
	for (int i=0; i<contours.nBlobs; i++) {
		ofCircle(contours.blobs[i].centroid.x, contours.blobs[i].centroid.y, 20);
	}
}

void pixelsApp::mousePressed(int x, int y, int button) {

	//calculate local mouse x,y in image
	int mx = x % w;
	int my = y % h;

	//get hue value on mouse position
	findHue = hue.getPixels()[my*w+mx];
}
