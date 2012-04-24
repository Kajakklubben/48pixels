#pragma once

#include "ofxOpenCv.h"
#include "coordWarp.h"
#include "rgb_to_hsv.h"
#include "ofxXmlSettings.h"


#ifndef TARGET_OSX
    //WIN
    //#define USE_UEYE
    #include "ofxUeye.h"
    #include "ofxUeyeSettings.h"
    //#define USE_COLOR_PROFILES
#else
    //OSX
#endif

#define CAM_W 640
#define CAM_H 480
#define NUM_BLOCKS 3
//#define USE_TRACKER



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
    void keyPressed(int key);

    bool isInDebug;
    ofTexture tex;


    //Sets the corners in 0...1 space
    void setCalibrationCorner(ofVec2f p, int corner);

    //Returns the pixel location for the block at x,y
    ofVec2f blockPixelLocationInCamera(float x, float y);

    string nameOfBlockColor(BlockColor color);

    Block blocks[8][6];

    int trackingAreaSize; //Pixels wide and high tracking rect
    double runningAverageAmount; //Lower is slower
    bool useCircleAverage;

    float colorMatchDistanceHue;//In degrees
    float colorMatchDistanceValue;
    float colorMatchDistanceSaturation;

private:

    unsigned char * pixels;

    unsigned char * pixelsR;
    unsigned char * pixelsG;
    unsigned char * pixelsB;
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
    ofxUeyeSettings uEyeSettings;

    void setAutoSettings();
    void setStaticSettings();
#else
    ofVideoGrabber videoGrabber;
#endif


};
