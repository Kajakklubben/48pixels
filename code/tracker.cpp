#include "tracker.h"

//------------------

void Tracker::setup(){
    #ifdef USE_UEYE
    initUeye();
    #else
    videoGrabber.initGrabber(CAM_W, CAM_H);
    #endif
    //Defeault settings
    setCalibrationCorner(ofVec2f(0.1,0.1), 0);
    setCalibrationCorner(ofVec2f(0.9,0.1), 1);
    setCalibrationCorner(ofVec2f(0.9,0.9), 2);
    setCalibrationCorner(ofVec2f(0.1,0.9), 3);

    handleHover = -1;
    handleSelected = -1;
    blockSelected.x = -1;

    for(int x=0;x<8;x++){
        for(int y=0;y<6;y++){
            blocks[x][y].runningAverageColor.r = 0;
            blocks[x][y].runningAverageColor.g = 0;
            blocks[x][y].runningAverageColor.b = 0;

            blocks[x][y].age = 0;
        }
    }

    for(int i=0;i<NUM_BLOCKS;i++){
        blockCalibrationColor[i].r = ofRandom(1);
        blockCalibrationColor[i].g = ofRandom(1);
        blockCalibrationColor[i].b = ofRandom(1);
    }
}


//------------------


void Tracker::setCalibrationCorner(ofVec2f p, int corner){
    if(corner >= 0 && corner < 4){
        calibrationCorners[corner] = p * ofVec2f(CAM_W,CAM_H);
        calibrationCorners[corner].x = MAX(0,MIN(CAM_W,calibrationCorners[corner].x));
        calibrationCorners[corner].y = MAX(0,MIN(CAM_H,calibrationCorners[corner].y));
    }

    ofVec2f src[4];
    src[0] = ofVec2f(0,0);
    src[1] = ofVec2f(7,0);
    src[2] = ofVec2f(7,5);
    src[3] = ofVec2f(0,5);

    coordWarper.calculateMatrix(src, calibrationCorners);
}

//------------------

ofVec2f Tracker::blockPixelLocationInCamera(int x, int y){
    return coordWarper.transform(x,y);
}

//------------------

string Tracker::nameOfBlockColor(BlockColor color){
    switch (color) {
        case BlockBlue:
            return "Blue";
            break;
        case BlockGreen:
            return "Green";
            break;
        case BlockBrown:
            return "Brown";
            break;

        default:
            return "N/A";
            break;
    }
}

//------------------

#ifdef USE_UEYE

void Tracker::initUeye()
{
    ofAddListener(ueye.events.dimensionChanged, this, &Tracker::ueyeDimensionChanged);

    ueye.setVerbose(true);
	ueye.listDevices();

	if(ueye.init())
	{
		//cout << (IS_BINNING_8X_VERTICAL | IS_BINNING_8X_HORIZONTAL) << endl;
		// Get full area of the sensor, but skipping every second pixel
		//ueye.setBinning(IS_BINNING_2X_VERTICAL | IS_BINNING_2X_HORIZONTAL); // difference from subsamplimg? (apparently same bandwith but smoother image)

		// smooth the bad pixels (apparently they come from factory with bad pixels...)
		ueye.enableBadPixelsCorrection();

		// Set AOI (always set AOI after binning, subsampling or scaler, otherwise you might not get the desired result)
		ofRectangle fullHD;
		fullHD.width = CAM_W;
		fullHD.height = CAM_H;
		fullHD.x = 0;
		fullHD.y = 0;
		//fullHD.y = (ueye.getAOIMax().height - fullHD.height) * 0.5;
		ueye.setAOI(fullHD);
		//ueye.setAOINormalized(ofRectangle(0,0, 0.6, 0.6));

		// Start grabbing pixels

       // ueye.enableAutoGain();
        //ueye.enableAutoWhiteBalance();
        //ueye.enableAutoExposureTime();

        //Almost optimal settings when sitting in labitat in dimmed light.
        ueye.setFPS(40);
        ueye.setGainMaster(96);
        ueye.setGainRed(0);
        ueye.setGainBlue(3);
        ueye.setGainBlue(35);

        ueye.setExposureTime(25.4);
        ueye.setPixelClock(18);
        ueye.enableLive();

		//settings.setup(&ueye);
	}

}
void Tracker::ueyeDimensionChanged(ofxUeyeEventArgs &args){
	// If we got here, bandwith has changed.
	// Pixel Clock, FPS and Exposure should be adjusted.
	//ueye.setPixelClock(ueye.getPixelClockMax());
	//ueye.setFPS(ueye.getFPSMax());
	//ueye.setFPS(60);

	tex.clear();
	tex.allocate(ueye.getWidth(), ueye.getHeight(),GL_RGB);
}

#endif

void Tracker::update(){

    #ifdef USE_UEYE
    ueye.update();
	//printf("\n ready %i newframe %i",ueye.isReady(),ueye.isFrameNew());
	if(ueye.isReady() )//&& ueye.isFrameNew())
	{
	    //printf("\n new frame");
	    tex.loadData(ueye.getPixels(), ueye.getWidth(), ueye.getHeight(), GL_RGB);
	    pixels = ueye.getPixels();
	}
	else return;
	#else
    videoGrabber.update();
    pixels = videoGrabber.getPixels();

	#endif

    for(int x=0;x<8;x++){
        for(int y=0;y<6;y++){
            //Find the local average for this block
            ofVec2f pixelLoc = blockPixelLocationInCamera(x, y);

            Color color;
            color.r = 0;
            color.g = 0;
            color.b = 0;

            for(int row=pixelLoc.y-trackingAreaSize/2 ; row<pixelLoc.y+trackingAreaSize/2 ; row++){
                unsigned char * pixel = (unsigned char*) pixels + int((row*CAM_W + (pixelLoc.x - trackingAreaSize/2)))*3;

                for(int i=0 ; i<trackingAreaSize ; i++, pixel += 3){
                    color.r += pixel[0]/255.0;
                    color.g += pixel[1]/255.0;
                    color.b += pixel[2]/255.0;
                }
            }

            color.r /= trackingAreaSize*trackingAreaSize;
            color.g /= trackingAreaSize*trackingAreaSize;
            color.b /= trackingAreaSize*trackingAreaSize;

            blocks[x][y].runningAverageColor.r = blocks[x][y].runningAverageColor.r * (1-runningAverageAmount) + (color.r * runningAverageAmount);
            blocks[x][y].runningAverageColor.g = blocks[x][y].runningAverageColor.g * (1-runningAverageAmount) + (color.g * runningAverageAmount);
            blocks[x][y].runningAverageColor.b = blocks[x][y].runningAverageColor.b * (1-runningAverageAmount) + (color.b * runningAverageAmount);


            //Match
            blocks[x][y].invalid = true;
            for(int i=0;i<NUM_BLOCKS;i++){
                ofVec3f v1 = ofVec3f(blocks[x][y].runningAverageColor.r, blocks[x][y].runningAverageColor.g, blocks[x][y].runningAverageColor.b);
                ofVec3f v2 = ofVec3f(blockCalibrationColor[i].r, blockCalibrationColor[i].g, blockCalibrationColor[i].b);

                float distance = v1.distance(v2);
                if(distance < colorMatchDistance){
                    if(blocks[x][y].blockColor != i){
                        blocks[x][y].age = 0;
                    }
                    blocks[x][y].blockColor = (BlockColor)i;
                    blocks[x][y].invalid = false;
                    blocks[x][y].matchDistance = distance;
                }
            }
            if(blocks[x][y].invalid){
                blocks[x][y].age = 0;
            } else {
                blocks[x][y].age++;
            }
        }
    }
}

//------------------

const int colorPickerY = 300;
const int colorPickerW = 200/(float)NUM_BLOCKS;

void Tracker::drawDebug(){
    float w = CAM_W;
    float h = CAM_H;

    ofSetColor(255, 255, 255);

    #ifdef USE_UEYE
    tex.draw(0,0,w,h);
    #else
    videoGrabber.draw(0, 0, w, h);
    #endif

    ofPushStyle(); {
        ofEnableAlphaBlending();

        //Draw block locations
        for(int x=0;x<8;x++){
            for(int y=0;y<6;y++){
                ofVec2f pos = blockPixelLocationInCamera(x, y);

                ofNoFill();
                ofSetLineWidth(1);
                //glColor3f(blocks[x][y].runningAverageColor.r, blocks[x][y].runningAverageColor.g, blocks[x][y].runningAverageColor.b);
                if(blocks[x][y].invalid){
                    ofSetColor(255, 255, 255,100);
                } else {
                    switch (blocks[x][y].blockColor) {
                        case BlockGreen:
                            ofSetColor(0, 255, 0);
                            break;
                        case BlockBrown:
                            ofSetColor(255, 100, 0);
                            break;
                        case BlockBlue:
                            ofSetColor(0, 0, 255);
                            break;
                        default:
                            break;
                    }
                }
                ofCircle(pos.x, pos.y, 7);

                if(blockSelected.x == x && blockSelected.y == y){
                    float a = sin(ofGetElapsedTimeMillis()/200.0);
                ofSetColor(150+a*80, 0, 0);
                ofCircle(pos.x, pos.y, 10+a*1.0);
                }

            }
        }

        //Draw handles
        int prevI = 3;
        for(int i=0;i<4;i++){
            //Handle
            ofFill();
            ofSetColor(255, 255, 255, 50);
            if(handleHover == i)
                ofSetColor(60, 60, 255, 150);
            ofCircle(calibrationCorners[i].x, calibrationCorners[i].y, 7);

            ofNoFill();
            ofSetColor(100, 100, 150, 255);
            ofCircle(calibrationCorners[i].x, calibrationCorners[i].y, 7);

            //Line between handles
            ofSetColor(100, 100, 100, 255);
            ofLine(calibrationCorners[i].x, calibrationCorners[i].y, calibrationCorners[prevI].x, calibrationCorners[prevI].y);

            prevI = i;
        }

    } ofPopStyle();


    //Info box
    int infoBoxX = 650;
    int infoBoxY = 0;
    if(blockSelected.x != -1){
        ofFill();
        ofSetColor(0, 0, 0,100);
        ofRect(infoBoxX, infoBoxY, 200, 480);

        ofNoFill();
        ofSetColor(255, 255, 255, 200);
        ofRect(infoBoxX, infoBoxY, 200, 480);

        ofSetColor(255, 255, 255);
        int y= 15;
        ofDrawBitmapString("Super Info Box", infoBoxX +5, infoBoxY + y);

        ofSetColor(220, 220, 220);

        y += 15;
        ofDrawBitmapString("Block "+ofToString(blockSelected.x)+":"+ofToString(blockSelected.y), infoBoxX +5, infoBoxY + y);

        Block block = blocks[(int)blockSelected.x][(int)blockSelected.y];

        y += 25;
        ofDrawBitmapString("r: "+ofToString(block.runningAverageColor.r,2)+"\n"+
                           "g: "+ofToString(block.runningAverageColor.g,2)+"\n"+
                           "b: "+ofToString(block.runningAverageColor.b,2), infoBoxX +5+30, infoBoxY + y);

        ofFill();
        glColor3f(block.runningAverageColor.r, block.runningAverageColor.g, block.runningAverageColor.b);
        ofRect(infoBoxX, y-10, 30, 40);
        ofSetColor(220, 220, 220);



        if(!block.invalid){
            y += 60;
            ofDrawBitmapString("Match: "+nameOfBlockColor(block.blockColor)+
                               "\nDistance: "+ofToString(block.matchDistance,2)+
                               "\nAge: "+ofToString(block.age), infoBoxX +5+30, infoBoxY + y);

            glColor3f(blockCalibrationColor[block.blockColor].r, blockCalibrationColor[block.blockColor].g, blockCalibrationColor[block.blockColor].b);
            ofRect(infoBoxX, y-10, 30, 40);
            ofSetColor(220, 220, 220);
        }

        ofFill();


        //Color picker
        y = colorPickerY;
        for(int i=0;i<NUM_BLOCKS;i++){
            ofSetColor(255, 255, 255);
            ofDrawBitmapString(nameOfBlockColor((BlockColor)i), ofPoint(i*colorPickerW+infoBoxX+3 , y-14));
            glColor3f(blockCalibrationColor[i].r, blockCalibrationColor[i].g, blockCalibrationColor[i].b);
            ofRect(i*colorPickerW+infoBoxX, y, colorPickerW, 50);
        }


    }

    ofSetColor(0, 0, 0);

}


void Tracker::mouseMoved(int x, int y ){
    handleHover = -1;
    for(int i=0;i<4;i++){
        ofVec2f mouse = ofVec2f(x,y);
        if(mouse.distance(calibrationCorners[i]) < 8){
            handleHover = i;
        }
    }


}
void Tracker::mouseDragged(int x, int y, int button){
    if(handleSelected >= 0){
        ofVec2f pos = ofVec2f((float)x/CAM_W, (float)y/CAM_H);

        setCalibrationCorner(pos, handleSelected);
    }
}
void Tracker::mousePressed(int x, int y, int button){
    if(x < CAM_W){
        handleSelected = -1;
        for(int i=0;i<4;i++){
            ofVec2f mouse = ofVec2f(x,y);
            if(mouse.distance(calibrationCorners[i]) < 8){
                handleSelected = i;
            }
        }

        blockSelected = ofVec2f(-1,-1);
        for(int bx=0;bx<8;bx++){
            for(int by=0;by<6;by++){
                ofVec2f mouse = ofVec2f(x,y);
                if(mouse.distance(blockPixelLocationInCamera(bx,by)) < 8){
                    blockSelected = ofVec2f(bx,by);
                }
            }
        }
    } else {
        if(y >= colorPickerY && y < colorPickerY + 50){
            for(int i=0;i<NUM_BLOCKS;i++){
                if(x - 650 < colorPickerW * (i+1)){
                    blockCalibrationColor[i].r = blocks[(int)blockSelected.x][(int)blockSelected.y].runningAverageColor.r;
                    blockCalibrationColor[i].g = blocks[(int)blockSelected.x][(int)blockSelected.y].runningAverageColor.g;
                    blockCalibrationColor[i].b = blocks[(int)blockSelected.x][(int)blockSelected.y].runningAverageColor.b;
                    break;
                }
            }
        }
    }
}
void Tracker::mouseReleased(int x, int y, int button){
    handleSelected = -1;
}


