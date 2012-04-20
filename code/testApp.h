#pragma once

#include "ofMain.h"
#include "tracker.h"
#include "calibrator.h"
#include "GameScreen.h"

class testApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    //---------------------

    Tracker tracker;
    Calibrator calibrator;
    gameScreen game;
    
    //---------------------
    
    bool trackerDebug;
    
    //---------------------


    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

};
