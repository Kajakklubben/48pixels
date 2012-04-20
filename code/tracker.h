#pragma once 

#include "ofxOpenCv.h"

enum BlockColor {
    BlockRed,
    BlockPoo
};

struct Block {
    BlockColor color;
    bool invalid;
};


//--------

class Tracker {

public:
    void setup();
    void update();
    void drawDebug();
    
    Block blocks[8][6];
  
private:
    ofVideoGrabber videoGrabber;
    
};