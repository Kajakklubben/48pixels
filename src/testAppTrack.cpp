#include "testAppTrack.h"

//--------------------------------------------------------------
void testAppTrack::setup(){

	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(320,240);
	#else
        vidPlayer.loadMovie("fingers.mov");
        vidPlayer.play();
	#endif

    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	colorBg.allocate(320,240);
	grayDiff.allocate(320,240);

	colourPixels  = new unsigned char[320*240*3];

	bLearnBakground = true;
	threshold = 40;
	minBlobSize = 100;
}

//--------------------------------------------------------------
void testAppTrack::update(){
	ofBackground(255,255,255);

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.idleMovie();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
            colourPixels = vidGrabber.getPixels();
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), 320,240);
        #endif


        grayImage = colorImg;
		if (bLearnBakground == true){
            colorBg.clear();
			grayBg = grayImage;
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);
		//grayDiff.erode();
      //  grayDiff.dilate();
       // grayDiff.dilate();
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
	}

}

//--------------------------------------------------------------
void testAppTrack::draw(){

	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
	colorImg.draw(20,20);
	grayImage.draw(360,20);
	grayBg.draw(20,280);
	grayDiff.draw(360,280);

	// then draw the contours:

	ofFill();
	ofSetHexColor(0x333333);
	ofRect(360,540,320,240);
	ofSetHexColor(0xffffff);

	// we could draw the whole contour finder
	//contourFinder.draw(360,540);

	// or, instead we can draw each blob individually,
	// this is how to get access to them:
	int width = 320;
	int height = 240;

    char blobInfoStr[1024];
    float r = 0;
    float g = 0;
    float b = 0;

    for (int i = 0; i < contourFinder.nBlobs; i++){
        if(contourFinder.blobs[i].area<minBlobSize)
            continue;

        contourFinder.blobs[i].draw(360,540);
        r = 0;
        g = 0;
        b = 0;


        int blobW = sqrt(contourFinder.blobs[i].area*0.6f);
        blobW = max(20,blobW);
        int blobH = blobW;

        int count = 0;

        for (int j = (int)contourFinder.blobs[i].centroid.x-blobW/2; j < (int)contourFinder.blobs[i].centroid.x+blobW/2; j++)
        {
            for (int k = (int)contourFinder.blobs[i].centroid.y-blobH/2; k < (int)contourFinder.blobs[i].centroid.y+blobH/2; k++)
            {
                int pos = (k * width + j);
                r += colourPixels[pos * 3];
                g += colourPixels[pos * 3+1];
                b += colourPixels[pos * 3+2];

                count++;
            }
        }

        r /=count;
        g /=count;
        b /=count;

        r = fmax(0,fmin(r,255));
        g = fmax(0,fmin(g,255));
        b = fmax(0,fmin(b,255));
        sprintf(blobInfoStr, "blob id: %i, r %f, g %f, b %f",i, r,g,b);
        ofDrawBitmapString(blobInfoStr, 400, 400+i*10);

        ofFill();
        ofSetColor(r,g,b);
        ofCircle(360+contourFinder.blobs[i].centroid.x,540+contourFinder.blobs[i].centroid.y,blobW/2);
        ofNoFill();

    }

	// finally, a report:
    ofSetColor(0,0,0);

	char reportStr[1024];
	sprintf(reportStr, "min blob size: %i \npress 'b' to capture bg\nthreshold %i (press: +/-)\nnum blobs found %i, fps: %f",minBlobSize ,threshold, contourFinder.nBlobs, ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 600);

}

//--------------------------------------------------------------
void testAppTrack::keyPressed(int key){

	switch (key){
		case 'b':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
		case 'n':
			minBlobSize ++;

			break;
		case 'm':
			minBlobSize --;

			break;
	}
}

//--------------------------------------------------------------
void testAppTrack::keyReleased(int key){

}

//--------------------------------------------------------------
void testAppTrack::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testAppTrack::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testAppTrack::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testAppTrack::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testAppTrack::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testAppTrack::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testAppTrack::dragEvent(ofDragInfo dragInfo){

}
