#pragma once 

#include "ofxOpenCv.h"
#include "coordWarp.h"

#define CAM_W 640
#define CAM_H 480

const int trackingAreaSize = 6; //Pixels wide and high tracking rect
const float runningAverageAmount = 0.01; //Lower is slower

enum BlockColor {
    BlockRed,
    BlockPoo
};

struct Color {
    float r;
    float g;
    float b;
};

struct Block {
    BlockColor blockColor;
    bool invalid;    
    int age;
    
    Color runningAverageColor;
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


    //Sets the corners in 0...1 space
    void setCalibrationCorner(ofVec2f p, int corner);
    
    //Returns the pixel location for the block at x,y
    ofVec2f blockPixelLocationInCamera(int x, int y);
    
    Block blocks[8][6];
  
private:
    ofVideoGrabber videoGrabber;
    coordWarping coordWarper; 
    
    //0 ... CAM_SIZE
    ofVec2f calibrationCorners[4];
    
    
    
    int handleHover, handleSelected;
    ofVec2f blockHover;
};