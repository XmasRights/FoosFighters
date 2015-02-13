 #include "pixelsApp.h"

//Pixel access is not implemented in the player
//so here is how to do it yourself

//it should be implemented along with an "isFrameNew" but that isn't working yet

//--------------------------------------------------------------
void pixelsApp::setup()
{
  w = 320;
  h = 240;

  anchoHue = anchoBri = anchoSat = 5;

  verHsb = verSat = verHue = false;
  verInput = true;

  videoGrabber.setup(w, h);

  //reserve memory for cv images
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
    //duplicate rgb
    hsb = rgb;

    //convert to hsb
    hsb.convertRgbToHsv();

    //store the three channels as grayscale images
    hsb.convertToGrayscalePlanarImages(hue, sat, bri);

    //filter image based on the hue value were looking for
    for (int i=0; i<w*h; i++) {
      if( ofInRange(hue.getPixels()[i],findHue-anchoHue,findHue+anchoHue) &&
          ofInRange(sat.getPixels()[i],findSat-anchoSat,findSat+anchoSat) &&
          ofInRange(bri.getPixels()[i],findBri-anchoBri,findBri+anchoBri)){
            filtered.getPixels()[i] =255;
        }else{
            filtered.getPixels()[i] =0;
        }
    }
    filtered.flagImageChanged();

    //run the contour finder on the filtered image to find blobs with a certain hue
    contours.findContours(filtered, 50, w*h/2, 1, false);
	}
}


//--------------------------------------------------------------
void pixelsApp::draw()
{
	if (videoGrabber.isFrameNew())
	{
		rgb.setFromPixels(videoGrabber.getPixels());
    rgb.mirror(false, true);
	}

  ofSetColor(255,255,255);

    //draw all cv images
    if(verInput) rgb.draw(0,0);
    if(verHsb) hsb.draw(0,0);
    if(verHue) hue.draw(0,0);
    if(verSat) sat.draw(0,0);
    filtered.draw(w,0);
    contours.draw(w,0);

    ofSetColor(255, 0, 0);
    ofFill();

    //draw red circles for found blobs
    for (int i=0; i<contours.nBlobs; i++) {
        ofCircle(contours.blobs[i].centroid.x, contours.blobs[i].centroid.y, 20);
    }
}


void pixelsApp::mousePressed(int x, int y, int button)
{
  if(x<w && y<h){
        //get hue value on mouse position
        findHue = hue.getPixels()[y*w+x];
        findSat = sat.getPixels()[y*w+x];
        findBri = bri.getPixels()[y*w+x];
    }
}

//--------------------------------------------------------------
void pixelsApp::keyPressed(int key){
  switch(key){
        case '1':
            verInput = true;
            verHsb = verSat = verHue = false;
            break;
        case '2':
            verHsb = true;
            verInput = verSat = verHue = false;
            break;
        case '3':
            verHue = true;
            verInput = verSat = verHsb = false;
            break;
        case '4':
            verSat = true;
            verInput = verHue = verHsb = false;
            break;
    case 'a':
      findHue = min(findHue + 1, 180);
      break;
    case 'z':
      findHue = max(findHue - 1, 0);
      break;
    case 's':
      findSat = min(findSat + 1, 100);
      break;
    case 'x':
      findSat = max(findSat - 1, 0);
      break;
    case 'd':
      findBri = min(findBri + 1, 100);
      break;
    case 'c':
      findBri = max(findBri - 1, 0);
      break;
    case 'f':
      anchoHue = min(anchoHue + 1, 180);
      break;
    case 'v':
      anchoHue = max(anchoHue - 1, 0);
      break;
    case 'g':
      anchoSat = min(anchoSat + 1, 100);
      break;
    case 'b':
      anchoSat = max(anchoSat - 1, 0);
      break;
    case 'h':
      anchoBri = min(anchoBri + 1, 100);
      break;
    case 'n':
      anchoBri = max(anchoBri - 1, 0);
      break;
  }
}
