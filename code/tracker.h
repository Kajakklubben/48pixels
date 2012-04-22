#pragma once

#include "ofxOpenCv.h"
#include "coordWarp.h"
#include "rgb_to_hsv.h"
#include "ofxXmlSettings.h"


#ifndef TARGET_OSX
    //WIN
    #define USE_UEYE
    #include "ofxUeye.h"
    #include "ofxUeyeSettings.h"
#else
    //OSX
#endif

#define CAM_W 640
#define CAM_H 480
#define NUM_BLOCKS 3




const int trackingAreaSize = 6; //Pixels wide and high tracking rect
const float runningAverageAmount = 0.01; //Lower is slower
const float colorMatchDistanceHue = 10; //In degrees
const float colorMatchDistanceValue = 0.3; 
const float colorMatchDistanceSaturation = 0.3; 

enum BlockColor {
    BlockBlue=0,
    BlockGreen=1,
    BlockBrown=2,
};

struct Block {
    BlockColor blockColor;
    bool invalid;
    int age;
    float matchDistance;

    rgb_color runningAverageColor;
};



//--------

class Tracker {

public:
    void setup();
    void update();
    void drawDebug();


    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);


    ofTexture tex;

    //Sets the corners in 0...1 space
    void setCalibrationCorner(ofVec2f p, int corner);

    //Returns the pixel location for the block at x,y
    ofVec2f blockPixelLocationInCamera(float x, float y);

    string nameOfBlockColor(BlockColor color);

    Block blocks[8][6];
    

private:

    unsigned char * pixels;
    coordWarping coordWarper;

    //0 ... CAM_SIZE
    ofVec2f calibrationCorners[4];

    rgb_color blockCalibrationColor[NUM_BLOCKS];

    int handleHover, handleSelected;
    ofVec2f blockSelected;
    
    ofxXmlSettings settings;
    
    
#ifdef USE_UEYE
    void ueyeDimensionChanged(ofxUeyeEventArgs &args);
    void initUeye();
    
    ofxUeye	ueye; // Only one instance of ofxUeye is allowed! ofxUeye is singleton.
    ofxUeyeSettings settings;
#else 
    ofVideoGrabber videoGrabber;
#endif
    

};
