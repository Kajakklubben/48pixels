#include "tracker.h"
//------------------

void Tracker::setup(){

    useCircleAverage = true;
    trackingAreaSize = 8; //Pixels wide and high tracking rect
    runningAverageAmount = 0.02; //Lower is slower

    isInDebug = false;
    //Initialize camera stuff
    #ifdef USE_UEYE
    initUeye();
    #else
    videoGrabber.initGrabber(CAM_W, CAM_H);
    #endif


if(settings.loadFile("../../../settings/settings.xml"))
    printf("settings.xml loaded for tracker settings!");

    settings.pushTag("root");
    settings.pushTag("settings");

     if(settings.attributeExists("trackSettings", "trackingAreaSize",0))
        trackingAreaSize = settings.getAttribute("trackSettings", "trackingAreaSize",8,0);


     if(settings.attributeExists("trackSettings", "runningAverageAmount",0))
        runningAverageAmount = settings.getAttribute("trackSettings", "runningAverageAmount",0.03,0);

     if(settings.attributeExists("trackSettings", "useCircleAverage",0))
        useCircleAverage = settings.getAttribute("trackSettings", "useCircleAverage",1,0);



    colorMatchDistanceHue = 10; //In deg
    colorMatchDistanceValue = 0.4;
    colorMatchDistanceSaturation = 0.32;    //Set defeault settings

     if(settings.attributeExists("trackSettings", "colorMatchDistanceHue",0))
        colorMatchDistanceHue = settings.getAttribute("trackSettings", "colorMatchDistanceHue",10,0);

     if(settings.attributeExists("trackSettings", "colorMatchDistanceValue",0))
        colorMatchDistanceValue = settings.getAttribute("trackSettings", "colorMatchDistanceValue",0.4,0);

     if(settings.attributeExists("trackSettings", "colorMatchDistanceSaturation",0))
        colorMatchDistanceSaturation = settings.getAttribute("trackSettings", "colorMatchDistanceSaturation",0.32,0);



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


    //    ofSetDataPathRoot(<#string root#>)
    //cout<<ofToDataPath(("../../../settings/settings.xml"),true)<<endl;




    if(settings.getNumTags("calibrationCorner") == 4){
        for(int i=0;i<4;i++){
            if(settings.attributeExists("calibrationCorner", "x",i)){
                ofVec2f p = ofVec2f(settings.getAttribute("calibrationCorner", "x", 0.0,i), settings.getAttribute("calibrationCorner", "y", 0.0,i));

                setCalibrationCorner(p, i);
            }
        }
    } else {
        ofLog(OF_LOG_FATAL_ERROR, "Setttings for calibration corners not found!");
    }

    if(settings.getNumTags("blockColor") == NUM_BLOCKS){
        for(int i=0;i<NUM_BLOCKS;i++){
            if(settings.attributeExists("blockColor", "r",i)){
                rgb_color c;
                c.r = settings.getAttribute("blockColor", "r", 0.0,i);
                c.g = settings.getAttribute("blockColor", "g", 0.0,i);
                c.b = settings.getAttribute("blockColor", "b", 0.0,i);

                blockCalibrationColor[i] = c;
            }
        }
    } else {
        ofLog(OF_LOG_FATAL_ERROR, "Setttings not found!");
    }



}

//------------------


void Tracker::keyPressed(int key)
{
    if(key=='d')
        isInDebug = !isInDebug ;

#ifdef USE_UEYE
    if(key=='a')
        setAutoSettings();

    if(key=='s')
        setStaticSettings();

#endif

    if(key=='c')
    {
        useCircleAverage = !useCircleAverage;
        settings.setAttribute("trackSettings", "useCircleAverage",useCircleAverage?1:0,0);
    }


    if(isInDebug && key=='u')
    {
        runningAverageAmount +=0.005;
        settings.setAttribute("trackSettings", "runningAverageAmount",runningAverageAmount,0);
    }
    if(isInDebug && key=='i')
    {
        runningAverageAmount -=0.005;
        settings.setAttribute("trackSettings", "runningAverageAmount",runningAverageAmount,0);
    }

    if(isInDebug && key=='t')
    {
        trackingAreaSize +=1;
        settings.setAttribute("trackSettings", "trackingAreaSize",trackingAreaSize,0);
    }
    if(isInDebug && key=='y')
    {
        trackingAreaSize -=1;
        settings.setAttribute("trackSettings", "trackingAreaSize",trackingAreaSize,0);
    }
    settings.saveFile();
}

void Tracker::update(){

#ifdef USE_UEYE
    {
        ueye.update();
        //printf("\n ready %i newframe %i",ueye.isReady(),ueye.isFrameNew());
        if(ueye.isReady() )//&& ueye.isFrameNew())
        {
            //printf("\n new frame");
            tex.loadData(ueye.getPixels(), ueye.getWidth(), ueye.getHeight(), GL_RGB);
            pixels = ueye.getPixels();
        }
        else return;
    }
#else
    {
        videoGrabber.update();
        pixels = videoGrabber.getPixels();
        tex = videoGrabber.getTextureReference();
    }
#endif

    for(int x=0;x<8;x++){
        for(int y=0;y<6;y++){
            //Find the local average for this block
            ofVec2f pixelLoc = blockPixelLocationInCamera(x, y);

            rgb_color color;
            color.r = 0;
            color.g = 0;
            color.b = 0;

            int countedPixels = 0;
            for(int row=pixelLoc.y-trackingAreaSize/2 ; row<pixelLoc.y+trackingAreaSize/2 ; row++){

                    unsigned char * pixel = (unsigned char*) pixels + int((row*CAM_W + (pixelLoc.x - trackingAreaSize/2)))*3;

                    for(int i=0 ; i<trackingAreaSize ; i++, pixel += 3){

                        if(!useCircleAverage || ofDist(pixelLoc.x- trackingAreaSize/2+i,row,pixelLoc.x,pixelLoc.y)<trackingAreaSize/2.0)
                        {
                            color.r += pixel[0]/255.0;
                            color.g += pixel[1]/255.0;
                            color.b += pixel[2]/255.0;
                            countedPixels++;
                        }
                    }
            }

            color.r /= countedPixels;
            color.g /= countedPixels;
            color.b /= countedPixels;

            blocks[x][y].runningAverageColor.r = blocks[x][y].runningAverageColor.r * (1-runningAverageAmount) + (color.r * runningAverageAmount);
            blocks[x][y].runningAverageColor.g = blocks[x][y].runningAverageColor.g * (1-runningAverageAmount) + (color.g * runningAverageAmount);
            blocks[x][y].runningAverageColor.b = blocks[x][y].runningAverageColor.b * (1-runningAverageAmount) + (color.b * runningAverageAmount);


            //Match
            blocks[x][y].invalid = true;

            //The average color of the block hole
            hsv_color hsvColor1 = rgb_to_hsv(blocks[x][y].runningAverageColor);

            for(int i=0;i<NUM_BLOCKS;i++){

                //The defined block color to match with
                hsv_color hsvColor2 = rgb_to_hsv(blockCalibrationColor[i]);

                float hueDistance = MIN(fabs(hsvColor1.hue - hsvColor2.hue), fabs(hsvColor1.hue - (hsvColor2.hue-360)));

                float satDistance = fabs(hsvColor1.sat - hsvColor2.sat);
                float valDistance = fabs(hsvColor1.val - hsvColor2.val);

                if(hueDistance < colorMatchDistanceHue &&
                   satDistance < colorMatchDistanceSaturation &&
                   valDistance < colorMatchDistanceValue){

                    if(blocks[x][y].blockColor != i){
                        blocks[x][y].age = 0;
                    }
                    blocks[x][y].blockColor = (BlockColor)i;
                    blocks[x][y].invalid = false;
                //    blocks[x][y].matchDistance = distance;
                }

                /*  float distance = v1.distance(v2);
                if(distance < colorMatchDistance){
                    if(blocks[x][y].blockColor != i){
                        blocks[x][y].age = 0;
                    }
                    blocks[x][y].blockColor = (BlockColor)i;
                    blocks[x][y].invalid = false;
                    blocks[x][y].matchDistance = distance;
                }*/
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

    //Settings
    if(settings.attributeExists("calibrationCorner", "x",corner)){
        settings.setAttribute("calibrationCorner", "x", p.x, corner);
        settings.setAttribute("calibrationCorner", "y", p.y, corner);

        settings.saveFile("../../../settings/settings.xml");
    }

    coordWarper.calculateMatrix(src, calibrationCorners);
}

//------------------

ofVec2f Tracker::blockPixelLocationInCamera(float x, float y){
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

const int colorPickerY = 380;
const int colorPickerW = 200/(float)NUM_BLOCKS;

void Tracker::drawDebug(){
    if(!isInDebug)
        return;
    float w = CAM_W;
    float h = CAM_H;



    ofFill();
    ofSetColor(0);
    ofRect(0,0,ofGetWidth(),ofGetHeight());

    ofFill();
    ofSetColor(0, 0, 0,150);
       //Info box
    int infoBoxX = 650;
    int infoBoxY = 0;
    ofRect(infoBoxX, infoBoxY, 200, 600);

    ofNoFill();
    ofSetColor(255, 255, 255, 200);
    ofRect(infoBoxX, infoBoxY, 200, 600);

    ofSetColor(255, 255, 255);
    int y= 15;
    ofDrawBitmapString("Super Info Box", infoBoxX +5, infoBoxY + y);


    ofSetColor(255, 255, 255);
    tex.draw(0,0,w,h);

    ofPushStyle(); {
        ofEnableAlphaBlending();

        //Draw block locations
        for(int x=0;x<8;x++){
            for(int y=0;y<6;y++){
                ofVec2f pos = blockPixelLocationInCamera(x, y);

                ofNoFill();
                ofSetLineWidth(1);

                //Small rect
                ofSetColor(200, 0, 0);
                if(useCircleAverage)
                    ofCircle(pos.x, pos.y, trackingAreaSize/2.0);
                else
                    ofRect(pos.x-trackingAreaSize*0.5, pos.y-trackingAreaSize*0.5, trackingAreaSize, trackingAreaSize);


                //glColor3f(blocks[x][y].runningAverageColor.r, blocks[x][y].runningAverageColor.g, blocks[x][y].runningAverageColor.b);
                if(blocks[x][y].invalid){
                    ofSetColor(0, 0, 0,100);
                } else {
                    switch (blocks[x][y].blockColor) {
                        case BlockGreen:
                            ofSetColor(0, 255, 0,100);
                            break;
                        case BlockBrown:
                            ofSetColor(255, 100, 0,100);
                            break;
                        case BlockBlue:
                            ofSetColor(0, 0, 255,100);
                            break;
                        default:
                            break;
                    }
                }


                //Big rect
                ofVec2f p1 = blockPixelLocationInCamera(x-0.5, y-0.5);
                ofVec2f p2 = blockPixelLocationInCamera(x+0.5, y-0.5);
                ofVec2f p3 = blockPixelLocationInCamera(x+0.5, y+0.5);
                ofVec2f p4 = blockPixelLocationInCamera(x-0.5, y+0.5);


                ofFill();

                glBegin(GL_QUADS);
                glVertex2d(p1.x, p1.y);
                glVertex2d(p2.x, p2.y);
                glVertex2d(p3.x, p3.y);
                glVertex2d(p4.x, p4.y);
                glEnd();


                ofSetColor(0, 0, 0);
                glBegin(GL_LINE_STRIP);
                glVertex2d(p1.x, p1.y);
                glVertex2d(p2.x, p2.y);
                glVertex2d(p3.x, p3.y);
                glVertex2d(p4.x, p4.y);
                glVertex2d(p1.x, p1.y);
                glEnd();

                ofNoFill();
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


    if(blockSelected.x != -1){
        ofNoFill();
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
//                               "\nDistance: "+ofToString(block.matchDistance,2)+
                               "\nAge: "+ofToString(block.age), infoBoxX +5+30, infoBoxY + y);

            glColor3f(blockCalibrationColor[block.blockColor].r, blockCalibrationColor[block.blockColor].g, blockCalibrationColor[block.blockColor].b);
            ofRect(infoBoxX, y-10, 30, 40);
            ofSetColor(220, 220, 220);
        }

        ofFill();



        //Zoom view
        int zoomY = 160;
        int width = 150;
        int pixelWidth = width/trackingAreaSize;

        ofPushStyle();{
            ofVec2f pixelLoc = blockPixelLocationInCamera(blockSelected.x, blockSelected.y);

            int y=0;
            ofFill();
            for(int row=pixelLoc.y-trackingAreaSize/2.0 ; row<pixelLoc.y+trackingAreaSize/2.0 ; row++){
                int x=0;
                unsigned char * pixel = (unsigned char*) pixels + int((row*CAM_W + (pixelLoc.x - trackingAreaSize/2.0)))*3;

                for(int i=0 ; i<trackingAreaSize ; i++, pixel += 3){

                    if(!useCircleAverage || ofDist(pixelLoc.x-trackingAreaSize/2.0+i,row,pixelLoc.x,pixelLoc.y)<trackingAreaSize/2.0)
                    {
                        ofSetColor(pixel[0], pixel[1], pixel[2],255);
                        ofRect(x*pixelWidth+600+width/2.0, y*pixelWidth+zoomY,pixelWidth, pixelWidth);
                    }
                    x++;
                }
                y++;
            }


        } ofPopStyle();



        //Color picker
        y = colorPickerY;

        hsv_color hsvColor1 = rgb_to_hsv(block.runningAverageColor);

        for(int i=0;i<NUM_BLOCKS;i++){
            ofSetColor(255, 255, 255);
            ofDrawBitmapString(nameOfBlockColor((BlockColor)i),
                               ofPoint(i*colorPickerW+infoBoxX+3 , y-14));
            glColor3f(blockCalibrationColor[i].r, blockCalibrationColor[i].g, blockCalibrationColor[i].b);
            ofRect(i*colorPickerW+infoBoxX, y, colorPickerW, 50);

            hsv_color hsvColor2 = rgb_to_hsv(blockCalibrationColor[i]);


            float hueDistance = MIN(fabs(hsvColor1.hue - hsvColor2.hue), fabs(hsvColor1.hue - (hsvColor2.hue-360)));
            float satDistance = fabs(hsvColor1.sat - hsvColor2.sat);
            float valDistance = fabs(hsvColor1.val - hsvColor2.val);

            ofSetColor(255, 255, 255);
            if(hueDistance < colorMatchDistanceHue) ofSetColor(0, 255, 0);

            ofDrawBitmapString("h: "+ofToString(hueDistance,1),
                               ofPoint(i*colorPickerW+infoBoxX+3 , y+65));

            ofSetColor(255, 255, 255);
            if(satDistance < colorMatchDistanceSaturation) ofSetColor(0, 255, 0);

            ofDrawBitmapString("\ns: "+ofToString(satDistance,2),
                               ofPoint(i*colorPickerW+infoBoxX+3 , y+65));

            ofSetColor(255, 255, 255);
            if(valDistance < colorMatchDistanceValue) ofSetColor(0, 255, 0);

            ofDrawBitmapString("\n\nv: "+ofToString(valDistance,2),
                               ofPoint(i*colorPickerW+infoBoxX+3 , y+65));

        }


    }

    //Showing current values
    ofSetColor(255, 255, 255);
    ofDrawBitmapString("trackingAreaSize (t/y):\n "+ofToString(trackingAreaSize,2),
                                ofPoint(653 , 510));

   ofDrawBitmapString("runningAverage (u/i):\n "+ofToString(runningAverageAmount,4),
                                ofPoint(652 , 540));

   ofDrawBitmapString("use cirle: (c):\n "+(string)(useCircleAverage?"True":"False"),
                                ofPoint(652 , 570));

    ofSetColor(0, 0, 0);

}

//-----------------------

#pragma mark - Camera Stuff

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
		//ueye.setAOI(ueye.getAOIMax());
		//ueye.setAOINormalized(ofRectangle(0,0, 0.6, 0.6));

        setStaticSettings();
        ueye.enableLive();

		//settings.setup(&ueye);
	}

}

void Tracker::setStaticSettings(){

 #ifdef USE_UEYE
    ueye.setFPS(40);
    ueye.setGainMaster(17);
    ueye.setGainRed(20);
    ueye.setGainGreen(14);
    ueye.setGainBlue(53);
    ueye.setAutoWhiteBalance(false);
    ueye.setAutoGain(false);

    ueye.setExposureTime(19.0);
    ueye.setPixelClock(32);
    //ueye.setPixelClock(ueye.getPixelClockMax());
 #endif

}

void Tracker::setAutoSettings(){

    #ifdef USE_UEYE
    ueye.setAutoExposureTime(true);
    ueye.setAutoWhiteBalance(true);
    ueye.setAutoGain(true);
    ueye.setPixelClock(ueye.getPixelClockMax());
    #endif
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


//------------------

#pragma mark - Mouse Stuff

void Tracker::mouseMoved(int x, int y ){
    if(!isInDebug)
        return;

    handleHover = -1;
    for(int i=0;i<4;i++){
        ofVec2f mouse = ofVec2f(x,y);
        if(mouse.distance(calibrationCorners[i]) < 8){
            handleHover = i;
        }
    }
}
void Tracker::mouseDragged(int x, int y, int button){
    if(!isInDebug)
        return;

    if(handleSelected >= 0){
        ofVec2f pos = ofVec2f((float)x/CAM_W, (float)y/CAM_H);

        setCalibrationCorner(pos, handleSelected);
    }
}
void Tracker::mousePressed(int x, int y, int button){

    if(!isInDebug)
        return;

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


                    //Settings
                    if(settings.attributeExists("blockColor", "r",i)){
                        settings.setAttribute("blockColor", "r", blockCalibrationColor[i].r, i);
                        settings.setAttribute("blockColor", "g", blockCalibrationColor[i].g, i);
                        settings.setAttribute("blockColor", "b", blockCalibrationColor[i].b, i);

                        settings.saveFile("../../../settings/settings.xml");
                    }


                    break;
                }
            }
        }
    }
}
void Tracker::mouseReleased(int x, int y, int button){
    handleSelected = -1;
}


